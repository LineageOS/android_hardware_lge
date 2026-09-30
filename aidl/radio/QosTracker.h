/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <map>
#include <set>

#include "Qos.h"

namespace lge::radio {
// Invoked on the adapter's single callback worker, never on binder threads.
class QosTracker {
  public:
    struct Call {
        int32_t cid;
        std::string identity;
        bool active;
    };
    void updateCalls(const std::vector<Call>& calls, int64_t nowMs);
    void setup(const Call& call, int64_t nowMs);
    void event(int32_t cid, int32_t qid, QosEvent event, std::optional<Session> session,
               bool hasPayload, int64_t nowMs);
    std::vector<Session> sessions(int32_t cid) const;
    void clear();

  private:
    struct Bearer {
        std::optional<Session> session;
        bool available = false;
    };
    struct State {
        std::string identity;
        std::map<int32_t, Bearer> bearers;
    };
    struct Pending {
        std::map<int32_t, Bearer> bearers;
        int64_t expires;
    };
    std::map<int32_t, State> mCalls;
    std::map<int32_t, Pending> mPending;
    std::set<int32_t> mRetired;
    static void apply(std::map<int32_t, Bearer>& bearers, int32_t qid, QosEvent event,
                      std::optional<Session> session, bool hasPayload);
    void expire(int64_t nowMs);
};
}  // namespace lge::radio
