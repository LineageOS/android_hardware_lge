/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/light/BnLights.h>

#include <map>
#include <mutex>
#include <fstream>

#include <lge_lights.h>

using ::aidl::android::hardware::light::FlashMode;
using ::aidl::android::hardware::light::HwLightState;
using ::aidl::android::hardware::light::HwLight;
using ::aidl::android::hardware::light::LightType;

namespace aidl {
namespace android {
namespace hardware {
namespace light {

class Lights : public BnLights {
public:
    Lights();

    ndk::ScopedAStatus setLightState(int32_t id, const HwLightState& state) override;
    ndk::ScopedAStatus getLights(std::vector<HwLight>* _aidl_return) override;

   private:
#ifdef LED
    void setLightLocked(const HwLightState& state);
    void checkLightStateLocked();
    void handleAttention(const HwLightState& state);
    void handleBattery(const HwLightState& state);
    void handleNotifications(const HwLightState& state);
#endif // LED
    void handleBacklight(const HwLightState& state);

    std::mutex globalLock;

#ifdef LED
    HwLightState mAttentionState;
    HwLightState mBatteryState;
    HwLightState mNotificationState;
#endif // LED

    std::map<LightType, std::function<void(const HwLightState&)>> mLights;

    int mMaxBrightness = 255;
    int mMaxBrightnessEx = 255;
};

} // namespace light
} // namespace hardware
} // namespace android
} // namespace aidl
