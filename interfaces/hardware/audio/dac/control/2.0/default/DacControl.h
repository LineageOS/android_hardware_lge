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

#pragma once

#include <vendor/lge/hardware/audio/dac/control/2.0/IDacControl.h>

#include <android/hardware/audio/2.0/IDevice.h>
#include <android/hardware/audio/2.0/IDevicesFactory.h>
#include <android/hardware/audio/4.0/IDevice.h>
#include <android/hardware/audio/4.0/IDevicesFactory.h>
#include <android/hardware/audio/5.0/IDevice.h>
#include <android/hardware/audio/5.0/IDevicesFactory.h>
#include <android/hardware/audio/6.0/IDevice.h>
#include <android/hardware/audio/6.0/IDevicesFactory.h>

#include <map>
#include <unordered_set>
#include <vector>

#include "Constants.h"

namespace vendor {
namespace lge {
namespace hardware {
namespace audio {
namespace dac {
namespace control {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::vendor::lge::hardware::audio::dac::control::V2_0::Feature;
using ::vendor::lge::hardware::audio::dac::control::V2_0::FeatureStates;
using ::vendor::lge::hardware::audio::dac::control::V2_0::FeatureState;

enum AudioVersion { V2_0, V4_0, V5_0, V6_0 };

class DacControl : public IDacControl {
  public:
    DacControl();

    Return<void> getSupportedFeatures(getSupportedFeatures_cb _hidl_cb) override;

    Return<void> getSupportedFeatureValues(Feature feature, getSupportedFeatureValues_cb _hidl_cb) override;

    Return<bool> getHifiDacState() override;

    Return<bool> setHifiDacState(bool enable) override;
    
    Return<bool> setFeatureValue(Feature feature, int32_t value) override;

    Return<int32_t> getFeatureValue(Feature feature) override;

    std::map<Feature, FeatureStates> mSupportedStates;
    std::vector<Feature> mSupportedFeatures;

  private:
    FeatureStates getAvcVolumeStates();
    FeatureStates getHifiModeStates();
    bool writeAvcVolumeState(int32_t value);
    bool writeHifiModeState(int32_t value);
    bool setAudioHALParameters(KeyValue kv);

    android::sp<::android::hardware::audio::V2_0::IDevice> mAudioDevice_V2_0;
    android::sp<::android::hardware::audio::V2_0::IDevicesFactory> mAudioDevicesFactory_V2_0;
    android::sp<::android::hardware::audio::V4_0::IDevice> mAudioDevice_V4_0;
    android::sp<::android::hardware::audio::V4_0::IDevicesFactory> mAudioDevicesFactory_V4_0;
    android::sp<::android::hardware::audio::V5_0::IDevice> mAudioDevice_V5_0;
    android::sp<::android::hardware::audio::V5_0::IDevicesFactory> mAudioDevicesFactory_V5_0;
    android::sp<::android::hardware::audio::V6_0::IDevice> mAudioDevice_V6_0;
    android::sp<::android::hardware::audio::V6_0::IDevicesFactory> mAudioDevicesFactory_V6_0;

    AudioVersion usedVersion;
  
    std::string mDacBasePath;
    std::string avcPath;
    std::string hifiPath;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace control
}  // namespace dac
}  // namespace audio
}  // namespace hardware
}  // namespace lge
}  // namespace vendor
