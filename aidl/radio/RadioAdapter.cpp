/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "RadioAdapter.h"

#include <android-base/logging.h>

#include <algorithm>
#include <chrono>

namespace lge::radio {
namespace {
int64_t nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
}
QosTracker::Call identity(const data::SetupDataCallResult& call) {
    std::string key = call.ifname;
    std::vector<std::string> addresses;
    for (const auto& address : call.addresses) addresses.push_back(address.address);
    std::sort(addresses.begin(), addresses.end());
    for (const auto& address : addresses) key += "|" + address;
    return {call.cid, key, call.active != 0};
}
data::QosSession toAidl(const Session& session) {
    data::QosSession result;
    result.qosSessionId = session.id;
    data::EpsQos eps;
    eps.qci = session.qci;
    eps.uplink.maxBitrateKbps = session.uplink.maximumKbps;
    eps.uplink.guaranteedBitrateKbps = session.uplink.guaranteedKbps;
    eps.downlink.maxBitrateKbps = session.downlink.maximumKbps;
    eps.downlink.guaranteedBitrateKbps = session.downlink.guaranteedKbps;
    result.qos.set<data::Qos::Tag::eps>(eps);
    for (const auto& f : session.filters) {
        data::QosFilter out;
        out.localAddresses = f.localAddresses;
        out.remoteAddresses = f.remoteAddresses;
        if (f.localPort) out.localPort = data::PortRange{f.localPort->start, f.localPort->end};
        if (f.remotePort) out.remotePort = data::PortRange{f.remotePort->start, f.remotePort->end};
        out.protocol = f.protocol;
        out.direction = f.direction;
        out.precedence = f.precedence;
        if (f.tos) out.tos.set<data::QosFilterTypeOfService::Tag::value>(*f.tos);
        if (f.flowLabel) out.flowLabel.set<data::QosFilterIpv6FlowLabel::Tag::value>(*f.flowLabel);
        if (f.spi) out.spi.set<data::QosFilterIpsecSpi::Tag::value>(*f.spi);
        result.qosFilters.push_back(std::move(out));
    }
    return result;
}
}  // namespace
RadioAdapter::RadioAdapter()
    : mWorker([this] {
          for (;;) {
              std::function<void()> work;
              {
                  std::unique_lock lock(mMutex);
                  mCv.wait(lock, [this] { return mStopping || !mWork.empty(); });
                  if (mStopping && mWork.empty()) return;
                  work = std::move(mWork.front());
                  mWork.pop_front();
              }
              work();  // No state mutex held over binder callbacks.
          }
      }) {}
RadioAdapter::~RadioAdapter() {
    {
        std::lock_guard lock(mMutex);
        mStopping = true;
    }
    mCv.notify_one();
    mWorker.join();
}
void RadioAdapter::post(std::function<void()> work) {
    {
        std::lock_guard lock(mMutex);
        CHECK_LT(mWork.size(), 256u) << "LG radio callback queue overflow";
        mWork.push_back(std::move(work));
    }
    mCv.notify_one();
}
void RadioAdapter::enrich(data::SetupDataCallResult& call) const {
    call.qosSessions.clear();
    for (const auto& session : mTracker.sessions(call.cid))
        call.qosSessions.push_back(toAidl(session));
}
void RadioAdapter::list(std::vector<data::SetupDataCallResult> calls) {
    std::vector<QosTracker::Call> identities;
    for (const auto& call : calls) identities.push_back(identity(call));
    mTracker.updateCalls(identities, nowMs());
    for (const auto& old : mCalls) {
        if (std::none_of(calls.begin(), calls.end(), [&](const auto& call) {
                return call.active != 0 && call.ifname == old.ifname;
            }))
            mRetiredInterfaces.insert(old.ifname);
    }
    mCalls = std::move(calls);
    std::vector<vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse> early;
    for (auto it = mEarly.begin(); it != mEarly.end();) {
        bool found = std::any_of(mCalls.begin(), mCalls.end(), [&](const auto& call) {
            return call.active != 0 && call.ifname == it->first.first;
        });
        if (found || it->second.second <= nowMs()) {
            if (found && !mRetiredInterfaces.count(it->first.first))
                early.push_back(it->second.first);
            it = mEarly.erase(it);
        } else
            ++it;
    }
    for (const auto& call : mCalls)
        if (call.active != 0) mRetiredInterfaces.erase(call.ifname);
    for (const auto& event : early) qos(event);
    for (auto& call : mCalls) enrich(call);
}
void RadioAdapter::setup(data::SetupDataCallResult call) {
    mTracker.setup(identity(call), nowMs());
    mCalls.erase(std::remove_if(mCalls.begin(), mCalls.end(),
                                [&](const auto& existing) { return existing.cid == call.cid; }),
                 mCalls.end());
    enrich(call);
    mCalls.push_back(std::move(call));
}
void RadioAdapter::reset() {
    mEps = false;
    mEarly.clear();
    mRetiredInterfaces.clear();
    mTracker.clear();
    emit();
    mCalls.clear();
}
void RadioAdapter::setEps(bool eps) {
    if (mEps && !eps) {
        mTracker.clear();
        mEarly.clear();
        emit();
    }
    mEps = eps;
}
void RadioAdapter::qos(const vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse& event) {
    LOG(DEBUG) << "LG data QoS interface=" << event.dev_name << " qid=" << event.qid
               << " status=" << event.status;
    auto call = std::find_if(mCalls.begin(), mCalls.end(), [&](const auto& call) {
        return call.active != 0 && call.ifname == event.dev_name.c_str();
    });
    if (call == mCalls.end()) {
        if (mEarly.size() < 32 && !mRetiredInterfaces.count(event.dev_name.c_str()))
            mEarly[{event.dev_name.c_str(), event.qid}] = {event, nowMs() + 5000};
        return;
    }
    applyQos(call->cid, event);
}
void RadioAdapter::emit() {
    for (auto& call : mCalls) enrich(call);
    if (!mCalls.empty() && onList) onList(mCalls);
}
void RadioAdapter::applyQos(int32_t cid,
                            const vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse& event) {
    // The stock serializer can encode LTE QCI or 5G 5QI with no discriminator.
    // Only expose EPS sessions while the standard radio reports an LTE anchor.
    if (!mEps) {
        LOG(WARNING) << "Ignoring ambiguous LG QoS outside LTE registration";
        return;
    }
    if (event.status < 1 || event.status > 6) {
        LOG(WARNING) << "Unknown LG QoS status " << event.status;
        return;
    }
    bool payload = !event.tx_flow_desc.empty() || !event.rx_flow_desc.empty() ||
                   !event.tx_tft.empty() || !event.rx_tft.empty();
    auto session = payload ? parseQos(event.qid, event.tx_flow_desc, event.rx_flow_desc,
                                      event.tx_tft, event.rx_tft)
                           : std::nullopt;
    if (payload && !session)
        LOG(WARNING) << "Rejected LG QoS payload cid=" << cid << " qid=" << event.qid;
    mTracker.event(cid, event.qid, static_cast<QosEvent>(event.status), std::move(session), payload,
                   nowMs());
    LOG(INFO) << "LG bearer update cid=" << cid << " qid=" << event.qid
              << " status=" << event.status << " sessions=" << mTracker.sessions(cid).size();
    emit();
}
}  // namespace lge::radio
