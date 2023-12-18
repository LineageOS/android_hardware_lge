/*
 * Copyright 2017 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.light@2.0-service.lge"

#include <hidl/HidlTransportSupport.h>
#include <utils/Errors.h>

#include "Light.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

using android::OK;
using android::sp;
using android::status_t;

int main() {
    bool hasBacklight = true;
    bool hasBlinkPattern = false;
    bool hasOnOffPattern = false;

    std::ofstream backlight(BL BRIGHTNESS);
    if (!backlight) {
        int error = errno;
        ALOGE("Failed to open %s (%d): %s", BL BRIGHTNESS, error, strerror(error));
        return -error;
    }

#ifdef LED
    std::ofstream emotionalBlinkPattern(LED BLINK_PATTERN);
    if (!emotionalBlinkPattern) {
        int error = errno;
        ALOGE("Failed to open %s (%d): %s", LED BLINK_PATTERN, error, strerror(error));
        ALOGE("Disable blink pattern");
        hasBlinkPattern = false;
    } else {
        hasBlinkPattern = true;
    }

    std::ofstream emotionalOnOffPattern(LED ONOFF_PATTERN);
    if (!emotionalOnOffPattern) {
        int error = errno;
        ALOGE("Failed to open %s (%d): %s", LED ONOFF_PATTERN, error, strerror(error));
        ALOGE("Disable onoff pattern");
        hasOnOffPattern = false;
    } else {
        hasOnOffPattern = true;
    }
#endif // LED

    android::sp<ILight> service = new Light(hasBacklight, hasBlinkPattern, hasOnOffPattern);

    configureRpcThreadpool(1, true);

    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Cannot register Light HAL service.");
        return 1;
    }

    ALOGI("Light HAL service ready.");

    joinRpcThreadpool();

    ALOGI("Light HAL service failed to join thread pool.");
    return 1;
}
