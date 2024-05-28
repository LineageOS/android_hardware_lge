/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay@2.0-service.lge"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include <livedisplay/lge/ColorEnhancement.h>
#include <livedisplay/lge/DisplayModes.h>
#include <livedisplay/lge/SunlightEnhancement.h>

#include <livedisplay/sdm/PictureAdjustment.h>

using android::OK;
using android::sp;
using android::status_t;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using ::vendor::lineage::livedisplay::V2_0::IColorEnhancement;
using ::vendor::lineage::livedisplay::V2_0::IDisplayModes;
using ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::ISunlightEnhancement;
using ::vendor::lineage::livedisplay::V2_0::implementation::ColorEnhancement;
using ::vendor::lineage::livedisplay::V2_0::implementation::DisplayModes;
using ::vendor::lineage::livedisplay::V2_0::implementation::SunlightEnhancement;

using ::vendor::lineage::livedisplay::V2_0::sdm::PictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::sdm::SDMController;

int main() {
    status_t status = OK;

    android::ProcessState::initWithDriver("/dev/vndbinder");

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();

    // HIDL frontend
    sp<ColorEnhancement> ce = new ColorEnhancement();;
    sp<DisplayModes> dm = new DisplayModes();
    sp<SunlightEnhancement> se = new SunlightEnhancement();
    sp<PictureAdjustment> pa = new PictureAdjustment(controller);

    if (ce == nullptr) {
        LOG(ERROR)
                << "Can not create an instance of LiveDisplay HAL ColorEnhancement Iface, exiting.";
        goto shutdown;
    }

    if (dm == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayModes Iface,"
                   << " exiting.";
        goto shutdown;
    }

    if (se == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL SunlightEnhancement Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status = dm->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Cannot register DisplayModes HAL service.";
        goto shutdown;
    }

    status = se->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for LiveDisplay HAL SunlightEnhancement Iface ("
                   << status << ")";
    }

    status = pa->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Cannot register PictureAdjustment HAL service.";
        goto shutdown;
    }

    status = ce->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Cannot register ColorEnhancement HAL service.";
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}
