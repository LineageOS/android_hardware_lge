/*
 * Copyright 2018 The LineageOS Project
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

#define LOG_TAG "android.hardware.ir@1.0-service.lge"

#include <hidl/HidlTransportSupport.h>
#include <dlfcn.h>

#include "ConsumerIr.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::ir::V1_0::IConsumerIr;
using android::hardware::ir::V1_0::implementation::ConsumerIr;

using android::OK;
using android::status_t;

#define LGE_CONSUMERIR_LIB "libcir_driver.so"

void * libcir_handle;
int (* transmitIr) (const char *, int, int, int *, int);

int main() {
    libcir_handle = dlopen(LGE_CONSUMERIR_LIB, RTLD_NOW);
    if (!libcir_handle) {
        ALOGE("%s: Failed to open %s: %s", __func__, LGE_CONSUMERIR_LIB, dlerror());
        return 1;
    }
	transmitIr = reinterpret_cast<typeof(transmitIr)>(dlsym(libcir_handle, "transmitIr"));
    if (!transmitIr) {
        ALOGE("%s: Failed to resolve function: %s: %s", __func__, "transmitIr", dlerror());
        return 1;
    }

    android::sp<IConsumerIr> service = new ConsumerIr();

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Cannot register ConsumerIr HAL service.");
        return 1;
    }

    ALOGI("ConsumerIr HAL service ready.");

    joinRpcThreadpool();

    ALOGI("ConsumerIr HAL service failed to join thread pool.");
    return 1;
}
