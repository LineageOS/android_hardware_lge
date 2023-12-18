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
    android::sp<ILight> service = new Light();

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
