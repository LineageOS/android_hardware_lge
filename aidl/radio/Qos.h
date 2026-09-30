/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace lge::radio {

// LG's QMI event enum, as serialized by lgdatavendorlib.so.
enum class QosEvent { ACTIVATED = 1, MODIFIED, DELETED, SUSPENDED, ENABLED, DISABLED };

struct Bandwidth {
    int32_t maximumKbps = 0;
    int32_t guaranteedKbps = 0;
};
struct PortRange {
    int32_t start;
    int32_t end;
};
struct Filter {
    std::vector<std::string> localAddresses;
    std::vector<std::string> remoteAddresses;
    std::optional<PortRange> localPort;
    std::optional<PortRange> remotePort;
    int8_t protocol = -1;
    int8_t direction = 0;
    int32_t precedence = -1;
    std::optional<uint8_t> tos;
    std::optional<int32_t> flowLabel;
    std::optional<int32_t> spi;
};
struct Session {
    int32_t id = 0;
    int32_t qci = 0;
    Bandwidth uplink;
    Bandwidth downlink;
    std::vector<Filter> filters;
};

// Never turn malformed or unrepresentable vendor filters into wildcard matches.
std::optional<Session> parseQos(int32_t id, const std::string& txFlow, const std::string& rxFlow,
                                const std::string& txTft, const std::string& rxTft);

}  // namespace lge::radio
