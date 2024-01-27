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

#include "DacControl.h"

#include <android-base/logging.h>
#include <android-base/strings.h>
#include <cutils/properties.h>

#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

namespace vendor {
namespace lge {
namespace hardware {
namespace audio {
namespace dac {
namespace control {
namespace V2_0 {
namespace implementation {

static constexpr int32_t MAX_BALANCE_VALUE = 0;
static constexpr int32_t MIN_BALANCE_VALUE = -12;

static std::vector<KeyValue> quaddac_states = {{"Off", "0"}, {"On", "1"}};

static std::vector<KeyValue> sound_presets = {{"Normal", "0"},
                                              {"Enhanced", "1"},
                                              {"Detailed", "2"},
                                              {"Live", "3"},
                                              {"Bass", "4"}};

static std::vector<KeyValue> digital_filters = {{"Short", "0"},
                                                {"Sharp", "1"},
                                                {"Slow", "2"}};

static std::vector<KeyValue> hifi_modes = {{"Normal", "0"},
                                           {"High Impedance", "1"},
                                           {"AUX", "2"}};

/*
 * Write value to path and close file.
 */
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

DacControl::DacControl() {
    DIR* dir;
    for(int i = 0; i < 10; i++) {
        std::string tmp = COMMON_ES9218_PATH + std::to_string(i) + "-0048/";
        if( (dir = opendir(tmp.c_str())) != NULL) {
            mDacBasePath.append(tmp);
            closedir(dir);
            break;
        }
        closedir(dir);
    }
    if(mDacBasePath.empty()) {
        LOG(ERROR) << "DacControl: No ES9218 path found, exiting...";
        return;
    }

    avcPath = std::string(mDacBasePath);
    avcPath.append(AVC_VOLUME);
    hifiPath = std::string(mDacBasePath);
    hifiPath.append(HIFI_MODE);

    mAudioDevicesFactory_V6_0 = ::android::hardware::audio::V6_0::IDevicesFactory::getService();
    if(mAudioDevicesFactory_V6_0 == nullptr) {
        LOG(INFO) << "mAudioDevicesFactory_V6_0 null, trying V5_0";
        mAudioDevicesFactory_V5_0 = ::android::hardware::audio::V5_0::IDevicesFactory::getService();

        if(mAudioDevicesFactory_V5_0 == nullptr) {
            LOG(INFO) << "mAudioDevicesFactory_V5_0 null, trying V4_0";
            mAudioDevicesFactory_V4_0 = ::android::hardware::audio::V4_0::IDevicesFactory::getService();

            if(mAudioDevicesFactory_V4_0 == nullptr) {
                LOG(INFO) << "mAudioDevicesFactory_V4_0 null, trying V2_0";
                mAudioDevicesFactory_V2_0 = ::android::hardware::audio::V2_0::IDevicesFactory::getService();

                if(mAudioDevicesFactory_V2_0 == nullptr) {
                    LOG(INFO) << "mAudioDevicesFactory_V2_0 null, aborting";
                    return;
                } else {
                    usedVersion = AudioVersion::V2_0;
                }
            } else {
                usedVersion = AudioVersion::V4_0;
            }
        } else {
            usedVersion = AudioVersion::V5_0;
        }
    } else {
        usedVersion = AudioVersion::V6_0;
    }

    switch(usedVersion) {
        case AudioVersion::V6_0: {
                std::function<void(::android::hardware::audio::V6_0::Result,
                    const android::sp<::android::hardware::audio::V6_0::IDevice>&)> openDevice_cb_V6_0 = [this](::android::hardware::audio::V6_0::Result result,
                                                                    const android::sp<::android::hardware::audio::V6_0::IDevice>& device)
                    {
                        if(result == ::android::hardware::audio::V6_0::Result::OK) {
                            this->mAudioDevice_V6_0 = device;
                        } else {
                            LOG(INFO) << "Couldnt open primary audio device";
                        }
                    };
                mAudioDevicesFactory_V6_0->openDevice("primary", openDevice_cb_V6_0);

                if(mAudioDevice_V6_0 == nullptr) {
                    LOG(INFO) << "mAudioDevice_V6_0 null, aborting";
                    return;
                }
            }
            break;
        case AudioVersion::V5_0: {
                std::function<void(::android::hardware::audio::V5_0::Result,
                    const android::sp<::android::hardware::audio::V5_0::IDevice>&)> openDevice_cb_V5_0 = [this](::android::hardware::audio::V5_0::Result result,
                                                                    const android::sp<::android::hardware::audio::V5_0::IDevice>& device)
                    {
                        if(result == ::android::hardware::audio::V5_0::Result::OK) {
                            this->mAudioDevice_V5_0 = device;
                        } else {
                            LOG(INFO) << "Couldnt open primary audio device";
                        }
                    };
                mAudioDevicesFactory_V5_0->openDevice("primary", openDevice_cb_V5_0);

                if(mAudioDevice_V5_0 == nullptr) {
                    LOG(INFO) << "mAudioDevice_V5_0 null, aborting";
                    return;
                }
            }
            break;
        case AudioVersion::V4_0: {
                std::function<void(::android::hardware::audio::V4_0::Result,
                    const android::sp<::android::hardware::audio::V4_0::IDevice>&)> openDevice_cb_V4_0 = [this](::android::hardware::audio::V4_0::Result result,
                                                                    const android::sp<::android::hardware::audio::V4_0::IDevice>& device)
                    {
                        if(result == ::android::hardware::audio::V4_0::Result::OK) {
                            this->mAudioDevice_V4_0 = device;
                        } else {
                            LOG(INFO) << "Couldnt open primary audio device";
                        }
                    };
                mAudioDevicesFactory_V4_0->openDevice("primary", openDevice_cb_V4_0);

                if(mAudioDevice_V4_0 == nullptr) {
                    LOG(INFO) << "mAudioDevice_V4_0 null, aborting";
                    return;
                }
            }
            break;
        case AudioVersion::V2_0: {
                std::function<void(::android::hardware::audio::V2_0::Result,
                    const android::sp<::android::hardware::audio::V2_0::IDevice>&)> openDevice_cb_V2_0 = [this](::android::hardware::audio::V2_0::Result result,
                                                                    const android::sp<::android::hardware::audio::V2_0::IDevice>& device)
                    {
                        if(result == ::android::hardware::audio::V2_0::Result::OK) {
                            this->mAudioDevice_V2_0 = device;
                        } else {
                            LOG(INFO) << "Couldnt open primary audio device";
                        }
                    };
                mAudioDevicesFactory_V2_0->openDevice(::android::hardware::audio::V2_0::IDevicesFactory::Device::PRIMARY, openDevice_cb_V2_0);

                if(mAudioDevice_V2_0 == nullptr) {
                    LOG(INFO) << "mAudioDevice_V2_0 null, aborting";
                    return;
                }
            }
            break;
        default: return; // Should never reach this state
    }

    /* Quad DAC */
    mSupportedFeatures.push_back(Feature::QuadDAC);
    FeatureStates quaddac_fstates;
    quaddac_fstates.states = hidl_vec<KeyValue> {quaddac_states};
    for(auto e : quaddac_fstates.states) {
        LOG(INFO) << "quaddac_fstates: " << e.name << ":" << e.value;
    }
    mSupportedStates.emplace(Feature::QuadDAC, quaddac_fstates);

    /* Digital Filter */
    mSupportedFeatures.push_back(Feature::DigitalFilter);
    FeatureStates digfilter_fstates;
    digfilter_fstates.states = hidl_vec<KeyValue> {digital_filters};
    mSupportedStates.emplace(Feature::DigitalFilter, digfilter_fstates);

    /* Sound Presets */
    mSupportedFeatures.push_back(Feature::SoundPreset);
    FeatureStates soundpresets_fstates;
    soundpresets_fstates.states = hidl_vec<KeyValue> {sound_presets};
    mSupportedStates.emplace(Feature::SoundPreset, soundpresets_fstates);

    /* Balance Left */
    mSupportedFeatures.push_back(Feature::BalanceLeft);
    FeatureStates balanceleft_fstates;
    balanceleft_fstates.range.max = MAX_BALANCE_VALUE;
    balanceleft_fstates.range.min = MIN_BALANCE_VALUE;
    balanceleft_fstates.range.step = 1;
    mSupportedStates.emplace(Feature::BalanceLeft, balanceleft_fstates);

    /* Balance Right */
    mSupportedFeatures.push_back(Feature::BalanceRight);
    FeatureStates balanceright_fstates;
    balanceright_fstates.range.max = MAX_BALANCE_VALUE;
    balanceright_fstates.range.min = MIN_BALANCE_VALUE;
    balanceright_fstates.range.step = 1;
    mSupportedStates.emplace(Feature::BalanceRight, balanceright_fstates);

    /* AVC Volume */
    struct stat buffer;
    if(stat(avcPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::AVCVolume);
        writeAvcVolumeState(getAvcVolumeState());
    }

    /* Hi-Fi Mode setting */
    if(stat(hifiPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::HifiMode);
        writeHifiModeState(getHifiModeState());
    }

    setFeatureValue(Feature::QuadDAC, getFeatureValue(Feature::QuadDAC));
    setFeatureValue(Feature::DigitalFilter, getFeatureValue(Feature::DigitalFilter));
    setFeatureValue(Feature::SoundPreset, getFeatureValue(Feature::SoundPreset));
    setFeatureValue(Feature::BalanceLeft, getFeatureValue(Feature::BalanceLeft));
    setFeatureValue(Feature::BalanceRight, getFeatureValue(Feature::BalanceRight));
}

Return<void> DacControl::getSupportedFeatures(getSupportedFeatures_cb _hidl_cb) {
    
    std::vector<Feature> ret;
    for(auto entry : mSupportedFeatures) {
        ret.push_back(entry);
    }
    _hidl_cb(ret);
    
    return Void();
}

FeatureStates DacControl::getAvcVolumeStates() {
    FeatureStates states;
    
    states.range.min = -24;
    states.range.max = 0;
    states.range.step = 1;
    
    return states;
}

FeatureStates DacControl::getHifiModeStates() {
    FeatureStates states;
    
    states.states = hidl_vec<KeyValue> {hifi_modes};
    
    return states;
}

Return<void> DacControl::getSupportedFeatureValues(Feature feature, getSupportedFeatureValues_cb _hidl_cb) {
    std::map<Feature, FeatureStates>::iterator it;

    // Check for sysfs-based features; these do different things
    switch(feature) {
        case Feature::AVCVolume: {
                _hidl_cb(getAvcVolumeStates());
                goto end;
            }
        case Feature::HifiMode: {
                _hidl_cb(getHifiModeStates());
                goto end;
            }
        default:
            break;
    }
    it = mSupportedStates.find(feature);
    if (it != mSupportedStates.end()) {
        _hidl_cb(it->second);
    } else {
        LOG(ERROR) << "DacControl::getSupportedFeatureValues: tried to get values for unsupported Feature...";
    }

end:
    return Void();
}

bool DacControl::writeAvcVolumeState(int32_t value) {
    set(avcPath, (-1)*value); //we save it as the actual value, while the kernel requires a positive value
    return (bool)property_set(PROPERTY_HIFI_DAC_AVC_VOLUME, std::to_string(value).c_str());
}

bool DacControl::writeHifiModeState(int32_t value) {
    set(hifiPath, value);
    return (bool)property_set(PROPERTY_HIFI_DAC_MODE, std::to_string(value).c_str());
}

Return<bool> DacControl::setFeatureValue(Feature feature, int32_t value) {

    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), feature) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::setFeatureValue: tried to set value for unsupported Feature...";
        return false;
    }

    KeyValue kv;
    std::string property;
    switch(feature) {
        case Feature::QuadDAC: {
            kv.name = DAC_COMMAND;
            property = PROPERTY_HIFI_DAC_ENABLED;
            if(value == 0) {
                kv.value = SET_DAC_OFF_COMMAND;
            } else if(value == 1) {
                kv.value = SET_DAC_ON_COMMAND;
            }
            goto set_parameters;
        }
        case Feature::DigitalFilter: {
            kv.name = SET_DIGITAL_FILTER_COMMAND;
            property = PROPERTY_DIGITAL_FILTER;
            break;
        }
        case Feature::SoundPreset: {
            kv.name = SET_SOUND_PRESET_COMMAND;
            property = PROPERTY_SOUND_PRESET;
            break;
        }
        case Feature::BalanceLeft: {
            kv.name = SET_LEFT_BALANCE_COMMAND;
            property = PROPERTY_LEFT_BALANCE;
            break;
        }
        case Feature::BalanceRight: {
            kv.name = SET_RIGHT_BALANCE_COMMAND;
            property = PROPERTY_RIGHT_BALANCE;
            break;
        }
        case Feature::AVCVolume: {
            return writeAvcVolumeState(value);
        }
        case Feature::HifiMode: {
            return writeHifiModeState(value);
        }
        default:
            return false;
    }
    kv.value = std::to_string(value);

set_parameters:
    ::android::hardware::audio::V2_0::Result result_V2_0 = ::android::hardware::audio::V2_0::Result::NOT_SUPPORTED;
    ::android::hardware::audio::V4_0::Result result_V4_0 = ::android::hardware::audio::V4_0::Result::NOT_SUPPORTED;
    ::android::hardware::audio::V5_0::Result result_V5_0 = ::android::hardware::audio::V5_0::Result::NOT_SUPPORTED;
    ::android::hardware::audio::V6_0::Result result_V6_0 = ::android::hardware::audio::V6_0::Result::NOT_SUPPORTED;

    switch(usedVersion) {
        case AudioVersion::V6_0: {
                std::vector<::android::hardware::audio::V6_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V6_0::ParameterValue> parameters_V6_0 =
                        hidl_vec<::android::hardware::audio::V6_0::ParameterValue> {pv_vec};
                result_V6_0 = mAudioDevice_V6_0->setParameters({}, parameters_V6_0);
            }
            break;
        case AudioVersion::V5_0: {
                std::vector<::android::hardware::audio::V5_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V5_0::ParameterValue> parameters_V5_0 =
                        hidl_vec<::android::hardware::audio::V5_0::ParameterValue> {pv_vec};
                result_V5_0 = mAudioDevice_V5_0->setParameters({}, parameters_V5_0);
            }
            break;
        case AudioVersion::V4_0: {
                std::vector<::android::hardware::audio::V4_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V4_0::ParameterValue> parameters_V4_0 =
                        hidl_vec<::android::hardware::audio::V4_0::ParameterValue> {pv_vec};
                result_V4_0 = mAudioDevice_V4_0->setParameters({}, parameters_V4_0);
            }
            break;
        case AudioVersion::V2_0: {
                std::vector<::android::hardware::audio::V2_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V2_0::ParameterValue> parameters_V2_0 =
                        hidl_vec<::android::hardware::audio::V2_0::ParameterValue> {pv_vec};
                result_V2_0 = mAudioDevice_V2_0->setParameters(parameters_V2_0);
            }
            break;
        default: return false;
    }

    if(result_V2_0 == ::android::hardware::audio::V2_0::Result::OK ||
       result_V4_0 == ::android::hardware::audio::V4_0::Result::OK ||
       result_V5_0 == ::android::hardware::audio::V5_0::Result::OK ||
       result_V6_0 == ::android::hardware::audio::V6_0::Result::OK) {
        // set by the audio HAL
        if(feature != Feature::QuadDAC) {
            property_set(property.c_str(), kv.value.c_str());
        }
        return true;
    } else {
        return false;
    }
}

int32_t DacControl::getAvcVolumeState() {
    return property_get_int32(PROPERTY_HIFI_DAC_AVC_VOLUME, AVC_VOLUME_DEFAULT);
}

int32_t DacControl::getHifiModeState() {
    return property_get_int32(PROPERTY_HIFI_DAC_MODE, HIFI_MODE_DEFAULT);
}

Return<int32_t> DacControl::getFeatureValue(Feature feature) {
    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), feature) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::getFeatureValue: tried to set value for unsupported Feature...";
        return -1;
    }

    int32_t ret;
    std::string property;
    char value[PROPERTY_VALUE_MAX];

    switch(feature) {
        case Feature::QuadDAC: {
            property = PROPERTY_HIFI_DAC_ENABLED;
            property_get(property.c_str(), value, PROPERTY_VALUE_HIFI_DAC_DISABLED);
            if(strcmp(value, PROPERTY_VALUE_HIFI_DAC_ENABLED) == 0) {
                ret = 1;
            } else {
                ret = 0;
            }
            goto end;
        }
        case Feature::DigitalFilter: {
            property = PROPERTY_DIGITAL_FILTER;
            break;
        }
        case Feature::SoundPreset: {
            property = PROPERTY_SOUND_PRESET;
            break;
        }
        case Feature::BalanceLeft: {
            property = PROPERTY_LEFT_BALANCE;
            break;
        }
        case Feature::BalanceRight: {
            property = PROPERTY_RIGHT_BALANCE;
            break;
        }
        case Feature::AVCVolume: {
            ret = getAvcVolumeState();
            goto end;
        }
        case Feature::HifiMode: {
            ret = getHifiModeState();
            goto end;
        }
        default:
            return false;
    }
    property_get(property.c_str(), value, "0");
    ret = std::stoi(value);

end:
    return ret;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace control
}  // namespace dac
}  // namespace audio
}  // namespace hardware
}  // namespace lge
}  // namespace vendor
