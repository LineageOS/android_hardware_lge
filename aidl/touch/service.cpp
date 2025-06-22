/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "GloveMode.h"
#include "TouchscreenGesture.h"

#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android-base/logging.h>

using ::aidl::vendor::lineage::touch::GloveMode;
using ::aidl::vendor::lineage::touch::TouchscreenGesture;

int main() {
    binder_status_t status = STATUS_OK;

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<GloveMode> glovemode = ndk::SharedRefBase::make<GloveMode>();

    const std::string glovemode_instance = std::string() + GloveMode::descriptor + "/default";
    status = AServiceManager_addService(glovemode->asBinder().get(), glovemode_instance.c_str());
    CHECK(status == STATUS_OK);

    std::shared_ptr<TouchscreenGesture> touchscreengesture = ndk::SharedRefBase::make<TouchscreenGesture>();
    const std::string touchscreengesture_instance = std::string() + TouchscreenGesture::descriptor + "/default";
    status = AServiceManager_addService(touchscreengesture->asBinder().get(), touchscreengesture_instance.c_str());
    CHECK(status == STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE; // should not reach
}
