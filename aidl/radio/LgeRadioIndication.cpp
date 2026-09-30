/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "LgeRadioIndication.h"

#include "RadioAdapter.h"
#include "commonStructs.h"
#include "data/structs.h"
#include "network/structs.h"

namespace android::hardware::radio::compat {
LgeRadioIndication::LgeRadioIndication(std::shared_ptr<DriverContext> context,
                                       std::shared_ptr<::lge::radio::RadioAdapter> adapter)
    : RadioIndication(std::move(context)), mAdapter(std::move(adapter)) {
    mAdapter->onList = [this](const auto& calls) {
        dataCb()->dataCallListChanged(
                ::aidl::android::hardware::radio::RadioIndicationType::UNSOLICITED, calls);
    };
}
Return<void> LgeRadioIndication::dataCallListChanged_1_5(
        V1_0::RadioIndicationType type, const hidl_vec<V1_5::SetupDataCallResult>& calls) {
    std::vector<::aidl::android::hardware::radio::data::SetupDataCallResult> converted;
    for (const auto& call : calls) converted.push_back(toAidl(call));
    mAdapter->post([this, type, calls = std::move(converted)]() mutable {
        mAdapter->list(std::move(calls));
        dataCb()->dataCallListChanged(toAidl(type), mAdapter->calls());
    });
    return {};
}
Return<void> LgeRadioIndication::currentSignalStrength_1_4(V1_0::RadioIndicationType type,
                                                           const V1_4::SignalStrength& signal) {
    networkCb()->currentSignalStrength(toAidl(type), toAidl(signal));
    return {};
}
Return<void> LgeRadioIndication::radioStateChanged(V1_0::RadioIndicationType type,
                                                   V1_0::RadioState state) {
    if (state != V1_0::RadioState::ON) mAdapter->post([this] { mAdapter->reset(); });
    modemCb()->radioStateChanged(toAidl(type),
                                 ::aidl::android::hardware::radio::modem::RadioState(state));
    return {};
}
Return<void> LgeRadioIndication::simStatusChanged(V1_0::RadioIndicationType type) {
    mAdapter->post([this] { mAdapter->reset(); });
    simCb()->simStatusChanged(toAidl(type));
    return {};
}
}  // namespace android::hardware::radio::compat
