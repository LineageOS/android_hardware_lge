/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "LgeRadioCompat.h"

#include <android-base/logging.h>

namespace android::hardware::radio::compat {
namespace {
template <class Target, class Source>
void bindCallbacks(Target& target, Source& source) {
    target.setResponseFunction(source.dataCb());
    target.setResponseFunction(source.messagingCb());
    target.setResponseFunction(source.modemCb());
    target.setResponseFunction(source.networkCb());
    target.setResponseFunction(source.simCb());
    target.setResponseFunction(source.voiceCb());
}
}  // namespace
LgeRadioCompat::LgeRadioCompat(sp<V1_5::IRadio> radio,
                               sp<vendor::lge::hardware::radio::V2_0::ILgeRadio> extension,
                               std::shared_ptr<DriverContext> context,
                               std::shared_ptr<::lge::radio::RadioAdapter> adapter)
    : V1_5::BsRadio(radio),
      mRadio(std::move(radio)),
      mExtension(std::move(extension)),
      mResponse(sp<LgeRadioResponse>::make(context, adapter)),
      mIndication(sp<LgeRadioIndication>::make(std::move(context), std::move(adapter))),
      mLgeResponse(sp<vendor::lge::hardware::radio::implementation::LgeRadioResponseV2>::make()),
      mLgeIndication(sp<vendor::lge::hardware::radio::implementation::LgeRadioIndicationV2>::make(
              mIndication)) {}
void LgeRadioCompat::setCallbackManager(std::weak_ptr<CallbackManager> callbacks) {
    mCallbacks = std::move(callbacks);
}
Return<void> LgeRadioCompat::setResponseFunctions(const sp<V1_0::IRadioResponse>&,
                                                  const sp<V1_0::IRadioIndication>&) {
    // Compat still batches framework registration. Refresh every domain on each
    // deferred registration so framework reconnection cannot retain old callbacks.
    auto callbacks = mCallbacks.lock();
    CHECK(callbacks);
    bindCallbacks(*mResponse, callbacks->response());
    bindCallbacks(*mIndication, callbacks->indication());
    mExtension->setResponseFunctions(mLgeResponse, mLgeIndication).assertOk();
    return mRadio->setResponseFunctions(mResponse, mIndication);
}
}  // namespace android::hardware::radio::compat
