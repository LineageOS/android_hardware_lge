/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Lights.h"

namespace aidl {
namespace android {
namespace hardware {
namespace light {

/*
 * Write value to path and close file.
 */
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

Lights::Lights() {
    auto backlightFn(std::bind(&Lights::handleBacklight, this, std::placeholders::_1));
    mLights.emplace(LightType::BACKLIGHT, backlightFn);

#ifdef LED
    auto attnFn(std::bind(&Lights::handleAttention, this, std::placeholders::_1));
    auto batteryFn(std::bind(&Lights::handleBattery, this, std::placeholders::_1));
    auto notifFn(std::bind(&Lights::handleNotifications, this, std::placeholders::_1));
    mLights.emplace(LightType::ATTENTION, attnFn);
    mLights.emplace(LightType::BATTERY, batteryFn);
    mLights.emplace(LightType::NOTIFICATIONS, notifFn);
#endif

    mMaxBrightness = get(BL MAX_BRIGHTNESS, -1);
    mMaxBrightnessEx = get(BL_EX MAX_BRIGHTNESS, -1);
    if (mMaxBrightness < 0) {
        mMaxBrightness = 255;
    }
    if (mMaxBrightnessEx < 0) {
        mMaxBrightnessEx = 255;
    }
}

static int rgbToBrightness(const HwLightState& state) {
    int color = state.color & 0x00ffffff;
    return ((77 * ((color >> 16) & 0x00ff))
            + (150 * ((color >> 8) & 0x00ff))
            + (29 * (color & 0x00ff))) >> 8;
}

#ifdef LED
static bool isLit(const HwLightState& state) {
    return (state.color & 0x00ffffff);
}

void Lights::setLightLocked(const HwLightState& state) {
    int onMS, offMS;
    uint32_t color;
    char pattern[PAGE_SIZE];

    switch (state.flashMode) {
        case Flash::TIMED:
            onMS = state.flashOnMs;
            offMS = state.flashOffMs;
            break;
        case Flash::NONE:
            onMS = 0;
            offMS = 0;
            break;
        default:
            onMS = -1;
            offMS = -1;
            break;
    }

    color = state.color & 0x00ffffff;

    if (offMS <= 0) {
        sprintf(pattern,"0x%06x", color);
        ALOGD("%s: Using onoff pattern: inColor=0x%06x\n", __func__, color);
        set(LED ONOFF_PATTERN, pattern);
    } else {
        sprintf(pattern,"0x%06x,%d,%d", color, onMS, offMS);
        ALOGD("%s: Using blink pattern: inColor=0x%06x delay_on=%d, delay_off=%d\n",
              __func__, color, onMS, offMS);
        set(LED BLINK_PATTERN, pattern);
    }
}

void Lights::checkLightStateLocked() {
    if (isLit(mNotificationState)) {
        setLightLocked(mNotificationState);
    } else if (isLit(mAttentionState)) {
        setLightLocked(mAttentionState);
    } else if (isLit(mBatteryState)) {
        setLightLocked(mBatteryState);
    } else {
        /* Lights off */
        set(LED BLINK_PATTERN, "0x0,-1,-1");
        set(LED ONOFF_PATTERN, "0x0");
    }
}

void Lights::handleAttention(const HwLightState& state) {
    mAttentionState = state;
    checkLightStateLocked();
}

void Lights::handleBattery(const HwLightState& state) {
    mBatteryState = state;
    checkLightStateLocked();
}

void Lights::handleNotifications(const HwLightState& state) {
    mNotificationState = state;
    checkLightStateLocked();
}
#endif // LED

void Lights::handleBacklight(const HwLightState& state) {
    int brightness, brightnessEx;
    int sentBrightness = rgbToBrightness(state);
    if(sentBrightness < 35) {
        brightness = sentBrightness * 2;
        brightnessEx = sentBrightness * 2;
    } else {
        brightness = sentBrightness * mMaxBrightness / 255;
        brightnessEx = sentBrightness * mMaxBrightnessEx / 255;
    }
    set(BL BRIGHTNESS, brightness);
    set(BL_EX BRIGHTNESS, brightnessEx);
}

ndk::ScopedAStatus Lights::setLightState(int32_t id, const HwLightState& state) {
    LightType type = static_cast<LightType>(id);
    auto it = mLights.find(type);

    if (it == mLights.end()) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    /*
     * Lock global mutex until light state is updated.
     */
    std::lock_guard<std::mutex> lock(globalLock);

    it->second(state);

    return ndk::ScopedAStatus::ok();
}

#define AutoHwLight(light) {.id = (int32_t)light, .type = light, .ordinal = 0}

ndk::ScopedAStatus Lights::getLights(std::vector<HwLight>* _aidl_return) {
    for (auto const& light : mLights)
        _aidl_return->push_back(AutoHwLight(light.first));

    return ndk::ScopedAStatus::ok();
}

} // namespace light
} // namespace hardware
} // namespace android
} // namespace aidl
