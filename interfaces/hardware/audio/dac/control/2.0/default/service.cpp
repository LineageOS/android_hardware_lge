/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vendor.lge.hardware.audio.dac.control@2.0-service"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "DacControl.h"

using android::OK;
using android::sp;
using android::status_t;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using ::vendor::lge::hardware::audio::dac::control::V2_0::IDacControl;
using ::vendor::lge::hardware::audio::dac::control::V2_0::implementation::DacControl;

int main() {
    sp<DacControl> interface;

    status_t status = OK;

    LOG(INFO) << "DAC Control HAL service is starting.";

    interface = new DacControl();
    if (interface == nullptr) {
        LOG(ERROR) << "Can not create an instance of DAC Control HAL DacControl Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);


    status = interface->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for DAC Control HAL DacControl Iface ("
                   << status << ")";
        goto shutdown;
    }

    LOG(INFO) << "DAC Control HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "DAC Control HAL service is shutting down.";
    return 1;
}
