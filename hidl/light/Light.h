/*
 * Copyright (C) 2017 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
#define ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H

#include <android/hardware/light/2.0/ILight.h>
#include <hardware/lights.h>
#include <hidl/Status.h>
#include <map>
#include <mutex>
#include <fstream>

#define BL              "/sys/class/backlight/panel0-backlight/"

#define BL_EX           "/sys/class/backlight/panel0-backlight-ex/"

#define LP_MODE         "/sys/devices/virtual/panel/factory/low_power_mode"

#define BRIGHTNESS      "brightness"
#define MAX_BRIGHTNESS  "max_brightness"

#define LED             "/sys/class/lg_rgb_led/use_patterns/"

#define BLINK_PATTERN   "blink_patterns"
#define ONOFF_PATTERN   "onoff_patterns"

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::light::V2_0::ILight;
using ::android::hardware::light::V2_0::LightState;
using ::android::hardware::light::V2_0::Status;
using ::android::hardware::light::V2_0::Type;

class Light : public ILight {
   public:
    Light(bool hasBacklight, bool hasBlinkPattern, bool hasOnOffPattern);

    Return<Status> setLight(Type type, const LightState& state) override;
    Return<void> getSupportedTypes(getSupportedTypes_cb _hidl_cb) override;

   private:
    void setLightLocked(const LightState& state);
    void checkLightStateLocked();
    void handleAttention(const LightState& state);
    void handleBacklight(const LightState& state);
    void handleBattery(const LightState& state);
    void handleNotifications(const LightState& state);

    std::mutex globalLock;

    LightState mAttentionState;
    LightState mBatteryState;
    LightState mNotificationState;

    std::map<Type, std::function<void(const LightState&)>> mLights;

    int mMaxBrightness = 255;
    int mMaxBrightnessEx = 255;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
