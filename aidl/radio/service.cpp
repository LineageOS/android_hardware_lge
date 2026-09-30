/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <hidl/HidlTransportSupport.h>
#include <libradiocompat/CallbackManager.h>
#include <libradiocompat/RadioConfig.h>
#include <libradiocompat/RadioData.h>
#include <libradiocompat/RadioMessaging.h>
#include <libradiocompat/RadioModem.h>
#include <libradiocompat/RadioNetwork.h>
#include <libradiocompat/RadioSim.h>
#include <libradiocompat/RadioVoice.h>
#include <vendor/lge/hardware/lgdata/1.0/ILgData.h>

#include "LgDataClient.h"
#include "LgeRadioCompat.h"
#include "RadioAdapter.h"

namespace {
namespace compat = android::hardware::radio::compat;
namespace hidl = android::hardware::radio;
class HalDeathRecipient : public android::hardware::hidl_death_recipient {
    void serviceDied(uint64_t, const android::wp<android::hidl::base::V1_0::IBase>&) override {
        LOG(FATAL) << "LG radio backend died; restarting";
    }
};
void linkDeath(const android::sp<android::hidl::base::V1_0::IBase>& service) {
    static const auto recipient = android::sp<HalDeathRecipient>::make();
    CHECK(service->linkToDeath(recipient, 0).withDefault(false));
}
std::vector<std::shared_ptr<ndk::ICInterface>> services;
std::vector<android::sp<lge::radio::LgDataClient>> dataClients;
template <class T, class... Args>
void publish(const std::string& slot, Args&&... args) {
    std::string name = std::string(T::descriptor) + "/" + slot;
    if (!AServiceManager_isDeclared(name.c_str())) return;
    auto service = ndk::SharedRefBase::make<T>(std::forward<Args>(args)...);
    CHECK_EQ(AServiceManager_addService(service->asBinder().get(), name.c_str()), STATUS_OK);
    services.push_back(service);
}
}  // namespace
int main() {
    android::base::InitLogging(nullptr, android::base::LogdLogger(android::base::RADIO));
    android::base::SetDefaultTag("lge-radio-aidl");
    android::hardware::configureRpcThreadpool(6, false);
    ABinderProcess_setThreadPoolMaxThreadCount(6);
    ABinderProcess_startThreadPool();
    auto config = hidl::config::V1_1::IRadioConfig::getService();
    CHECK(config);
    linkDeath(config);
    publish<compat::RadioConfig>("default", config);
    const int slots = android::base::GetIntProperty("ro.boot.vendor.lge.sim_num", 1);
    CHECK_GE(slots, 1);
    CHECK_LE(slots, 3);
    for (int slot = 1; slot <= slots; ++slot) {
        auto name = "slot" + std::to_string(slot);
        auto real = hidl::V1_5::IRadio::getService(name);
        CHECK(real);
        linkDeath(real);
        auto context = std::make_shared<compat::DriverContext>();
        auto adapter = std::make_shared<lge::radio::RadioAdapter>();
        auto extension = vendor::lge::hardware::radio::V2_0::ILgeRadio::getService(
                "lge_radio" + (slot == 1 ? std::string() : std::to_string(slot)));
        CHECK(extension) << "LG radio extension is required for signal reporting";
        linkDeath(extension);
        auto backend = android::sp<compat::LgeRadioCompat>::make(real, extension, context, adapter);
        auto callbacks = std::make_shared<compat::CallbackManager>(context, backend);
        backend->setCallbackManager(callbacks);
        publish<compat::RadioData>(name, context, backend, callbacks);
        publish<compat::RadioMessaging>(name, context, backend, callbacks);
        publish<compat::RadioModem>(name, context, backend, callbacks);
        publish<compat::RadioNetwork>(name, context, backend, callbacks);
        publish<compat::RadioSim>(name, context, backend, callbacks);
        publish<compat::RadioVoice>(name, context, backend, callbacks);
        auto dataService = vendor::lge::hardware::lgdata::V1_0::ILgData::getService();
        CHECK(dataService) << "LG data service is required for bearer reporting";
        linkDeath(dataService);
        auto client = android::sp<lge::radio::LgDataClient>::make(adapter);
        dataClients.push_back(client);
        dataService->setCallback(slot - 1, client).assertOk();
        LOG(INFO) << "Registered LG data QoS callback for " << name;
    }
    LOG(INFO) << "LG AIDL radio ready; bearer state requires real LG notifications";
    ABinderProcess_joinThreadPool();
    return 1;
}
