/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service-lge"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>

#include <livedisplay/lge/ColorEnhancement.h>
#include <livedisplay/lge/DisplayModes.h>
#include <livedisplay/lge/SunlightEnhancement.h>

#include <livedisplay/sdm/PictureAdjustment.h>

using ::aidl::vendor::lineage::livedisplay::lge::ColorEnhancement;
using ::aidl::vendor::lineage::livedisplay::lge::DisplayModes;
using ::aidl::vendor::lineage::livedisplay::lge::SunlightEnhancement;

using ::aidl::vendor::lineage::livedisplay::sdm::PictureAdjustment;
using ::aidl::vendor::lineage::livedisplay::sdm::SDMController;


int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();

    // AIDL frontend
    std::shared_ptr<ColorEnhancement> ce = ndk::SharedRefBase::make<ColorEnhancement>();
    std::shared_ptr<DisplayModes> dm = ndk::SharedRefBase::make<DisplayModes>();
    std::shared_ptr<SunlightEnhancement> se = ndk::SharedRefBase::make<SunlightEnhancement>();
    std::shared_ptr<PictureAdjustment> pa = ndk::SharedRefBase::make<PictureAdjustment>(controller);
    binder_status_t status;

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    if (ce == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL ColorEnhancement Iface, "
                      "exiting.";
        goto shutdown;
    }

    if (dm == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayModes Iface,"
                   << " exiting.";
        goto shutdown;
    }

    if (se == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL SunlightEnhancement Iface, "
                      "exiting.";
        goto shutdown;
    }

    if (ce->isSupported()) {
        std::string instance = std::string(ColorEnhancement::descriptor) + "/default";
        status = AServiceManager_addService(ce->asBinder().get(), instance.c_str());
        if (status != STATUS_OK) {
            LOG(ERROR) << "Cannot register ColorEnhancement HAL service.";
            goto shutdown;
        }
    }

    if (dm->isSupported()) {
        std::string instance = std::string(DisplayModes::descriptor) + "/default";
        status = AServiceManager_addService(dm->asBinder().get(), instance.c_str());
        if (status != STATUS_OK) {
            LOG(ERROR) << "Cannot register DisplayModes HAL service.";
            goto shutdown;
        }
    }

    if (se->isSupported()) {
        std::string instance = std::string(SunlightEnhancement::descriptor) + "/default";
        status = AServiceManager_addService(se->asBinder().get(), instance.c_str());
        if (status != STATUS_OK) {
            LOG(ERROR) << "Cannot register SunlightEnhancement HAL service.";
            goto shutdown;
        }
    }

    if (pa) {
        std::string instance = std::string(PictureAdjustment::descriptor) + "/default";
        status = AServiceManager_addService(pa->asBinder().get(), instance.c_str());
        if (status != STATUS_OK) {
            LOG(ERROR) << "Cannot register PictureAdjustment HAL service.";
            goto shutdown;
        }
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    ABinderProcess_joinThreadPool();

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return EXIT_FAILURE;
}