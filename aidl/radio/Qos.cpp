/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "Qos.h"

#include <arpa/inet.h>

#include <charconv>
#include <climits>
#include <map>
#include <set>
#include <string_view>

namespace lge::radio {
namespace {
using Fields = std::map<std::string, std::string>;

std::string_view trim(std::string_view s) {
    while (!s.empty() && s.front() == ' ') s.remove_prefix(1);
    while (!s.empty() && s.back() == ' ') s.remove_suffix(1);
    return s;
}

std::optional<uint64_t> number(std::string_view s, uint64_t maximum) {
    s = trim(s);
    int base = 10;
    if (s.substr(0, 2) == "0x") {
        base = 16;
        s.remove_prefix(2);
    }
    uint64_t value;
    auto [end, error] = std::from_chars(s.data(), s.data() + s.size(), value, base);
    if (error != std::errc() || end != s.data() + s.size() || value > maximum) return {};
    return value;
}

std::optional<Fields> fields(std::string_view text) {
    Fields result;
    while (!text.empty()) {
        auto comma = text.find(',');
        auto token = trim(text.substr(0, comma));
        auto equals = token.find('=');
        if (equals == std::string_view::npos) return {};
        auto key = trim(token.substr(0, equals));
        auto value = trim(token.substr(equals + 1));
        if (key.empty() || value.empty() || !result.emplace(key, value).second) return {};
        if (comma == std::string_view::npos) break;
        text.remove_prefix(comma + 1);
        if (text.empty()) return {};
    }
    return result;
}

bool flow(const std::string& text, int32_t& qci, Bandwidth& bandwidth) {
    if (text.empty()) return true;  // A unidirectional bearer is representable.
    auto values = fields(text);
    if (!values || values->size() != 3) return false;
    auto c = number((*values)["qci"], 254);
    auto m = number((*values)["max_rate"], uint64_t(INT_MAX) * 1000);
    auto g = number((*values)["guaranteed_rate"], uint64_t(INT_MAX) * 1000);
    if (!c || !*c || !m || !g || *g > *m || (qci && static_cast<uint64_t>(qci) != *c)) return false;
    qci = *c;
    // QMI data_rate_min_max is in bits/s; Android bandwidth fields are Kbit/s.
    bandwidth = {static_cast<int32_t>(*m / 1000), static_cast<int32_t>(*g / 1000)};
    return true;
}

bool address(Fields& values, const std::string& version, const std::string& side,
             std::vector<std::string>& addresses) {
    const auto addrKey = version + "_" + side + "_addr";
    const auto maskKey =
            version + "_" + side + (version == "ipv4" ? "_subnet_mask" : "_filter_prefix_len");
    auto addr = values.find(addrKey);
    auto mask = values.find(maskKey);
    if (addr == values.end()) return mask == values.end();
    if (mask == values.end()) return false;
    unsigned char buffer[16];
    if (inet_pton(version == "ipv4" ? AF_INET : AF_INET6, addr->second.c_str(), buffer) != 1)
        return false;
    auto n = number(mask->second, version == "ipv4" ? UINT32_MAX : 128);
    if (!n) return false;
    unsigned prefix = *n;
    if (version == "ipv4") {
        uint32_t bits = *n;
        prefix = 0;
        while (bits & 0x80000000u) {
            ++prefix;
            bits <<= 1;
        }
        if (bits) return false;
    }
    addresses.push_back(addr->second + "/" + std::to_string(prefix));
    values.erase(addr);
    values.erase(mask);
    return true;
}

bool ports(Fields& values, const std::string& transport, const std::string& side,
           std::optional<PortRange>& output) {
    const auto key = transport + "_" + side + "_port_start";
    // LG's TCP source key really is spelled "ports_range".
    const auto rangeKey = transport + "_" + side +
                          (transport == "tcp" && side == "src" ? "_ports_range" : "_port_range");
    auto start = values.find(key);
    auto range = values.find(rangeKey);
    if (start == values.end()) return range == values.end();
    if (range == values.end() || output) return false;
    auto s = number(start->second, 65535);
    auto r = number(range->second, 65535);
    if (!s || !r || *s < 20 || *s + *r > 65535) return false;
    output = PortRange{static_cast<int32_t>(*s), static_cast<int32_t>(*s + *r)};
    values.erase(start);
    values.erase(range);
    return true;
}

bool byteMask(Fields& values, const std::string& valueKey, const std::string& maskKey,
              std::optional<uint8_t>& output) {
    auto value = values.find(valueKey);
    auto mask = values.find(maskKey);
    if (value == values.end()) return mask == values.end();
    if (mask == values.end()) return false;
    auto v = number(value->second, 255);
    auto m = number(mask->second, 255);
    // The standard HAL cannot preserve a partial mask. Reject, rather than broaden it.
    if (!v || !m || (*m != 0 && *m != 255)) return false;
    if (*m) {
        if (output && *output != *v) return false;
        output = *v;
    }
    values.erase(value);
    values.erase(mask);
    return true;
}

bool filters(const std::string& text, bool transmit, std::vector<Filter>& output) {
    if (text.empty()) return true;
    std::string_view remaining = text;
    while (!remaining.empty()) {
        if (output.size() >= 32) return false;
        auto slash = remaining.find('/');
        auto parsed = fields(remaining.substr(0, slash));
        if (!parsed || parsed->empty()) return false;
        auto& values = *parsed;
        Filter filter;
        filter.direction = transmit ? 1 : 0;
        for (const auto& version : {"ipv4", "ipv6"}) {
            if (!address(values, version, "src",
                         transmit ? filter.localAddresses : filter.remoteAddresses) ||
                !address(values, version, "dest",
                         transmit ? filter.remoteAddresses : filter.localAddresses))
                return false;
        }
        auto protocol = values.find("protocol");
        if (protocol != values.end()) {
            auto p = number(protocol->second, 255);
            if (!p || (*p != 6 && *p != 17 && *p != 50 && *p != 51)) return false;
            filter.protocol = *p;
            values.erase(protocol);
        }
        for (const auto& transport : {"tcp", "udp", "transport"}) {
            bool hasPorts = values.count(std::string(transport) + "_src_port_start") ||
                            values.count(std::string(transport) + "_dest_port_start");
            if (hasPorts && std::string_view(transport) != "transport") {
                int expected = std::string_view(transport) == "tcp" ? 6 : 17;
                if (filter.protocol != -1 && filter.protocol != expected) return false;
                filter.protocol = expected;
            }
            if (!ports(values, transport, "src", transmit ? filter.localPort : filter.remotePort) ||
                !ports(values, transport, "dest", transmit ? filter.remotePort : filter.localPort))
                return false;
        }
        if (!byteMask(values, "tos_value", "mask", filter.tos) ||
            !byteMask(values, "ipv6_traffic_class_value", "ipv6_traffic_class_mask", filter.tos))
            return false;
        for (const auto& key :
             {"filter_id", "precedence", "ipv6_flow_label", "eps_security_policy"}) {
            auto it = values.find(key);
            if (it == values.end()) continue;
            auto n = number(it->second,
                            std::string_view(key) == "ipv6_flow_label" ? 0xfffff : UINT32_MAX);
            if (!n) return false;
            if (std::string_view(key) == "precedence") {
                if (*n > INT_MAX) return false;
                filter.precedence = *n;
            } else if (std::string_view(key) == "ipv6_flow_label")
                filter.flowLabel = *n;
            else if (std::string_view(key) == "eps_security_policy")
                filter.spi = static_cast<int32_t>(*n);
            values.erase(it);
        }
        if (!values.empty()) return false;
        // QosCallbackTracker needs an address, port or protocol to match a socket.
        if (filter.localAddresses.empty() && filter.remoteAddresses.empty() && !filter.localPort &&
            !filter.remotePort && filter.protocol == -1)
            return false;
        output.push_back(std::move(filter));
        if (slash == std::string_view::npos) break;
        remaining.remove_prefix(slash + 1);
        if (remaining.empty()) return false;
    }
    return true;
}
}  // namespace

std::optional<Session> parseQos(int32_t id, const std::string& txFlow, const std::string& rxFlow,
                                const std::string& txTft, const std::string& rxTft) {
    if (id <= 0 || txFlow.size() > 127 || rxFlow.size() > 127 || txTft.size() > 2047 ||
        rxTft.size() > 2047)
        return {};
    Session session;
    session.id = id;
    if (!flow(txFlow, session.qci, session.uplink) ||
        !flow(rxFlow, session.qci, session.downlink) || !session.qci ||
        !filters(txTft, true, session.filters) || !filters(rxTft, false, session.filters) ||
        session.filters.empty())
        return {};
    return session;
}
}  // namespace lge::radio
