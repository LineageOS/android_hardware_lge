/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <android/hardware/radio/1.5/BsRadio.h>
#include <libradiocompat/CallbackManager.h>
#include <vendor/lge/hardware/radio/2.0/ILgeRadio.h>

#include "LgeRadioIndication.h"
#include "LgeRadioIndicationV2.h"
#include "LgeRadioResponse.h"
#include "LgeRadioResponseV2.h"

namespace android::hardware::radio::compat {
// Generated forwarding keeps all requests upstream; only callback wiring is LG-specific.
class LgeRadioCompat : public V1_5::BsRadio {
    // This adapter implements 1.5, even if its vendor backend also exposes 1.6.
    Return<void> interfaceChain(interfaceChain_cb callback) override {
        return V1_5::IRadio::interfaceChain(std::move(callback));
    }
    Return<void> setResponseFunctions(const sp<V1_0::IRadioResponse>&,
                                      const sp<V1_0::IRadioIndication>&) override;
    sp<V1_5::IRadio> mRadio;
    sp<vendor::lge::hardware::radio::V2_0::ILgeRadio> mExtension;
    sp<LgeRadioResponse> mResponse;
    sp<LgeRadioIndication> mIndication;
    sp<vendor::lge::hardware::radio::implementation::LgeRadioResponseV2> mLgeResponse;
    sp<vendor::lge::hardware::radio::implementation::LgeRadioIndicationV2> mLgeIndication;
    std::weak_ptr<CallbackManager> mCallbacks;

  public:
    LgeRadioCompat(sp<V1_5::IRadio> radio,
                   sp<vendor::lge::hardware::radio::V2_0::ILgeRadio> extension,
                   std::shared_ptr<DriverContext> context,
                   std::shared_ptr<::lge::radio::RadioAdapter> adapter);
    void setCallbackManager(std::weak_ptr<CallbackManager> callbacks);
};
}  // namespace android::hardware::radio::compat
