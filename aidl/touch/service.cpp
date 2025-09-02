/*
 * SPDX-FileCopyrightText: 2022-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.lge"

#include "GloveMode.h"
#include "TouchscreenGesture.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::touch::GloveMode;
using aidl::vendor::lineage::touch::TouchscreenGesture;

int main() {
    LOG(INFO) << "Starting Lineage Touch HAL Service";

    binder_status_t status = STATUS_OK;
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    // Register GloveMode if supported
    std::shared_ptr<GloveMode> gm = ndk::SharedRefBase::make<GloveMode>();
    if (gm->isSupported()) {
        const std::string gm_instance = std::string(GloveMode::descriptor) + "/default";
        status = AServiceManager_addService(gm->asBinder().get(), gm_instance.c_str());
        if (status != STATUS_OK) {
            LOG(ERROR) << "Failed to register " << gm_instance << ": " << status;
        } else {
            LOG(INFO) << "Registered " << gm_instance;
        }
    } else {
        LOG(INFO) << "GloveMode not supported on this device";
    }

    // Register TouchscreenGesture if supported
    std::shared_ptr<TouchscreenGesture> tg = ndk::SharedRefBase::make<TouchscreenGesture>();
    if (tg->isSupported()) {
        const std::string tg_instance = std::string(TouchscreenGesture::descriptor) + "/default";
        status = AServiceManager_addService(tg->asBinder().get(), tg_instance.c_str());
        if (status != STATUS_OK) {
            LOG(ERROR) << "Failed to register " << tg_instance << ": " << status;
        } else {
            LOG(INFO) << "Registered " << tg_instance;
        }
    } else {
        LOG(INFO) << "TouchscreenGesture not supported on this device";
    }

    LOG(INFO) << "Lineage Touch HAL Service ready — joining binder thread pool";
    ABinderProcess_joinThreadPool();
    LOG(ERROR) << "Service exited binder thread pool — should not happen";
    return EXIT_FAILURE; // should not reach
}