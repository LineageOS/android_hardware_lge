/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <vendor/lge/hardware/lgdata/1.0/ILgDataCallback.h>

#include "RadioAdapter.h"
namespace lge::radio {
using ::android::hardware::Return;
// The AP IMS replacement owns the stock LG data callback for this slot.
class LgDataClient : public vendor::lge::hardware::lgdata::V1_0::ILgDataCallback {
  public:
    explicit LgDataClient(std::shared_ptr<RadioAdapter> adapter) : mAdapter(std::move(adapter)) {}
    Return<void> iwlanCellularQualityChangedInd(int32_t, int32_t) override { return {}; }
    Return<void> handoverToCellularInd(
            const vendor::lge::hardware::lgdata::V1_0::Handover2CellularInd&) override {
        return {};
    }
    Return<void> dataQosChangedInd(
            const vendor::lge::hardware::lgdata::V1_0::LgDataQosResponse& event) override {
        mAdapter->post([adapter = mAdapter, event] { adapter->qos(event); });
        return {};
    }

  private:
    std::shared_ptr<RadioAdapter> mAdapter;
};
}  // namespace lge::radio
