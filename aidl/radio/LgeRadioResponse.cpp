/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "LgeRadioResponse.h"

#include "RadioAdapter.h"
#include "commonStructs.h"
#include "data/structs.h"
#include "network/structs.h"

namespace android::hardware::radio::compat {
LgeRadioResponse::LgeRadioResponse(std::shared_ptr<DriverContext> context,
                                   std::shared_ptr<::lge::radio::RadioAdapter> adapter)
    : RadioResponse(std::move(context)), mAdapter(std::move(adapter)) {}
Return<void> LgeRadioResponse::getDataCallListResponse_1_5(
        const V1_0::RadioResponseInfo& info, const hidl_vec<V1_5::SetupDataCallResult>& calls) {
    std::vector<::aidl::android::hardware::radio::data::SetupDataCallResult> converted;
    for (const auto& call : calls) converted.push_back(toAidl(call));
    mAdapter->post([this, info, calls = std::move(converted)]() mutable {
        if (info.error == V1_0::RadioError::NONE) mAdapter->list(calls);
        for (auto& call : calls) mAdapter->enrich(call);
        dataCb()->getDataCallListResponse(toAidl(info), calls);
    });
    return {};
}
Return<void> LgeRadioResponse::setupDataCallResponse_1_5(const V1_0::RadioResponseInfo& info,
                                                         const V1_5::SetupDataCallResult& call) {
    mAdapter->post([this, info, call = toAidl(call)]() mutable {
        if (info.error == V1_0::RadioError::NONE &&
            call.cause == ::aidl::android::hardware::radio::data::DataCallFailCause::NONE &&
            call.active != 0)
            mAdapter->setup(call);
        mAdapter->enrich(call);
        dataCb()->setupDataCallResponse(toAidl(info), call);
    });
    return {};
}
Return<void> LgeRadioResponse::getDataRegistrationStateResponse_1_5(
        const V1_0::RadioResponseInfo& info, const V1_5::RegStateResult& result) {
    mAdapter->post([this, info, result] {
        if (info.error == V1_0::RadioError::NONE)
            mAdapter->setEps(result.rat == V1_4::RadioTechnology::LTE ||
                             result.rat == V1_4::RadioTechnology::LTE_CA);
        networkCb()->getDataRegistrationStateResponse(toAidl(info), toAidl(result));
    });
    return {};
}
}  // namespace android::hardware::radio::compat
