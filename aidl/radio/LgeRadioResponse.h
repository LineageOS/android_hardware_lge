/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <libradiocompat/RadioResponse.h>

namespace lge::radio {
class RadioAdapter;
}
namespace android::hardware::radio::compat {
class LgeRadioResponse : public RadioResponse {
    Return<void> getDataCallListResponse_1_5(
            const V1_0::RadioResponseInfo& info,
            const hidl_vec<V1_5::SetupDataCallResult>& calls) override;
    Return<void> setupDataCallResponse_1_5(const V1_0::RadioResponseInfo& info,
                                           const V1_5::SetupDataCallResult& call) override;
    Return<void> getDataRegistrationStateResponse_1_5(const V1_0::RadioResponseInfo& info,
                                                      const V1_5::RegStateResult& result) override;
    std::shared_ptr<::lge::radio::RadioAdapter> mAdapter;

  public:
    LgeRadioResponse(std::shared_ptr<DriverContext> context,
                     std::shared_ptr<::lge::radio::RadioAdapter> adapter);
};
}  // namespace android::hardware::radio::compat
