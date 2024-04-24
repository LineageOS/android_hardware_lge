/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.radio@1.4-service.lge"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <hidl/HidlTransportSupport.h>

#include "Radio.h"
#include "hidl-utils.h"

#include <map>

using namespace android::hardware::radio;
using namespace android::hardware::hidl_utils;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::base::GetIntProperty;

using android::hardware::radio::implementation::Radio;

using android::OK;
using android::sp;
using android::status_t;

int main() {
    // Note: Starts from slot 1
    std::map<int, sp<V1_4::IRadio>> slotIdToRadio;
    int slotCount = GetIntProperty("ro.boot.vendor.lge.sim_num", 1);

    for (int slotId = 1; slotId <= slotCount; slotId++) {
        sp<V1_0::IRadio> realRadio = V1_0::IRadio::getService("slot" + std::to_string(slotId));
        if (realRadio == nullptr) {
            LOG(INFO) << "Cannot get radio service for slot " << slotId;

            if (slotId == 1) {
                LOG(ERROR) << "Cannot get radio service for slot 1.";
                return 1;
            }

            break;
        }

        slotIdToRadio[slotId] = new Radio(realRadio);
        linkDeathToDeath(realRadio);
    }

    configureRpcThreadpool(slotCount * 2 + 2, true);

    for (auto const& [slotId, radio] : slotIdToRadio) {
        status_t status = radio->registerAsService("slot" + std::to_string(slotId));
        if (status != OK) {
            LOG(ERROR) << "Cannot register Radio HAL service for slot " << slotId;
            return 1;
        }
    }

    LOG(INFO) << "Radio HAL service ready.";

    joinRpcThreadpool();

    LOG(INFO) << "Radio HAL service failed to join thread pool.";
    return 1;
}
