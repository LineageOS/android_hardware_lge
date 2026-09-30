/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <aidl/android/hardware/radio/data/SetupDataCallResult.h>
#include <vendor/lge/hardware/lgdata/1.0/types.h>

#include <condition_variable>
#include <deque>
#include <functional>
#include <thread>

#include "QosTracker.h"

namespace lge::radio {
namespace data = ::aidl::android::hardware::radio::data;

class RadioAdapter {
  public:
    RadioAdapter();
    ~RadioAdapter();
    void post(std::function<void()> work);
    void qos(const vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse& event);
    void setEps(bool eps);
    void list(std::vector<data::SetupDataCallResult> calls);
    void setup(data::SetupDataCallResult call);
    void reset();
    void enrich(data::SetupDataCallResult& call) const;
    void emit();
    const std::vector<data::SetupDataCallResult>& calls() const { return mCalls; }
    std::function<void(const std::vector<data::SetupDataCallResult>&)> onList;

  private:
    void applyQos(int32_t cid, const vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse& event);
    std::mutex mMutex;
    std::condition_variable mCv;
    bool mStopping = false;
    std::deque<std::function<void()>> mWork;
    std::thread mWorker;
    bool mEps = false;
    std::map<std::pair<std::string, int32_t>,
             std::pair<vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse, int64_t>>
            mEarly;
    std::set<std::string> mRetiredInterfaces;
    QosTracker mTracker;
    std::vector<data::SetupDataCallResult> mCalls;
};

}  // namespace lge::radio
