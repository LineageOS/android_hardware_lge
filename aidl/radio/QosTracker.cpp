/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "QosTracker.h"

#include <algorithm>

namespace lge::radio {
void QosTracker::expire(int64_t nowMs) {
    for (auto it = mPending.begin(); it != mPending.end();) {
        if (it->second.expires <= nowMs)
            it = mPending.erase(it);
        else
            ++it;
    }
}
void QosTracker::clear() {
    mCalls.clear();
    mPending.clear();
    mRetired.clear();
}
void QosTracker::setup(const Call& call, int64_t nowMs) {
    // A successful setup is a new generation, even if the CID was reused.
    mCalls.erase(call.cid);
    updateCalls(
            [&] {
                std::vector<Call> calls{call};
                for (const auto& [cid, state] : mCalls)
                    calls.push_back({cid, state.identity, true});
                return calls;
            }(),
            nowMs);
}
void QosTracker::updateCalls(const std::vector<Call>& calls, int64_t nowMs) {
    expire(nowMs);
    std::set<int32_t> present;
    for (const auto& call : calls) {
        if (!call.active) continue;
        present.insert(call.cid);
        auto existing = mCalls.find(call.cid);
        if (existing != mCalls.end() && existing->second.identity == call.identity) continue;
        bool reused = existing != mCalls.end() || mRetired.count(call.cid);
        State state{call.identity, {}};
        auto early = mPending.find(call.cid);
        if (!reused && early != mPending.end()) state.bearers = std::move(early->second.bearers);
        mPending.erase(call.cid);
        mCalls[call.cid] = std::move(state);
        mRetired.erase(call.cid);
    }
    for (auto it = mCalls.begin(); it != mCalls.end();) {
        if (!present.count(it->first)) {
            if (mRetired.size() < 256) mRetired.insert(it->first);
            mPending.erase(it->first);
            it = mCalls.erase(it);
        } else
            ++it;
    }
}
void QosTracker::apply(std::map<int32_t, Bearer>& bearers, int32_t qid, QosEvent event,
                       std::optional<Session> session, bool hasPayload) {
    if (qid <= 0) return;
    if (event == QosEvent::DELETED) {
        bearers.erase(qid);
        return;
    }
    if (event == QosEvent::SUSPENDED || event == QosEvent::DISABLED) {
        auto it = bearers.find(qid);
        if (it != bearers.end()) it->second.available = false;
        return;
    }
    if (event != QosEvent::ACTIVATED && event != QosEvent::MODIFIED && event != QosEvent::ENABLED)
        return;
    if (hasPayload || event == QosEvent::ACTIVATED || event == QosEvent::MODIFIED) {
        // Invalid updates revoke availability, rather than retaining stale matching filters.
        if (!session || session->id != qid) {
            bearers.erase(qid);
            return;
        }
        if (!bearers.count(qid) && bearers.size() >= 32) return;
        bearers[qid] = {std::move(session), true};
    } else {
        auto it = bearers.find(qid);
        if (it != bearers.end() && it->second.session) it->second.available = true;
    }
}
void QosTracker::event(int32_t cid, int32_t qid, QosEvent event, std::optional<Session> session,
                       bool hasPayload, int64_t nowMs) {
    expire(nowMs);
    if (cid < 0 || qid <= 0) return;
    auto it = mCalls.find(cid);
    if (it != mCalls.end()) {
        apply(it->second.bearers, qid, event, std::move(session), hasPayload);
    } else if (!mRetired.count(cid)) {
        if (!mPending.count(cid) && mPending.size() >= 8) return;
        auto [p, inserted] = mPending.try_emplace(cid, Pending{{}, nowMs + 5000});
        (void)inserted;
        apply(p->second.bearers, qid, event, std::move(session), hasPayload);
    }
}
std::vector<Session> QosTracker::sessions(int32_t cid) const {
    std::vector<Session> result;
    auto it = mCalls.find(cid);
    if (it != mCalls.end())
        for (const auto& [qid, bearer] : it->second.bearers) {
            (void)qid;
            if (bearer.available && bearer.session) result.push_back(*bearer.session);
        }
    return result;
}
}  // namespace lge::radio
