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

std::vector<std::string> PROPERTY_CUSTOM_FILTER_COEFFS = {
    "persist.vendor.audio.ess.customFilterCoeff0", "persist.vendor.audio.ess.customFilterCoeff1",
    "persist.vendor.audio.ess.customFilterCoeff2", "persist.vendor.audio.ess.customFilterCoeff3", "persist.vendor.audio.ess.customFilterCoeff4",
    "persist.vendor.audio.ess.customFilterCoeff5", "persist.vendor.audio.ess.customFilterCoeff6", "persist.vendor.audio.ess.customFilterCoeff7",
    "persist.vendor.audio.ess.customFilterCoeff8", "persist.vendor.audio.ess.customFilterCoeff9", "persist.vendor.audio.ess.customFilterCoeff10",
    "persist.vendor.audio.ess.customFilterCoeff11", "persist.vendor.audio.ess.customFilterCoeff12", "persist.vendor.audio.ess.customFilterCoeff13" 
};

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
                                           {"AUX High Impedance", "2"}};

/*
 * Write value to path and close file.
 */
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

DacControl::DacControl() {
    if(access(COMMON_ES9218_PATH, F_OK) != 0) {
        LOG(ERROR) << "DacControl: No ES9218 path found, exiting...";
        return;
    }

    struct stat buffer;

    avcPath = std::string(COMMON_ES9218_PATH);
    avcPath.append(AVC_VOLUME);
    volumeLeftPath = std::string(COMMON_ES9218_PATH);
    volumeLeftPath.append(VOLUME_LEFT);
    volumeRightPath = std::string(COMMON_ES9218_PATH);
    volumeRightPath.append(VOLUME_RIGHT);
    hifiPath = std::string(COMMON_ES9218_PATH);
    hifiPath.append(HIFI_MODE);
    essFilterPath = std::string(COMMON_ES9218_PATH);
    essFilterPath.append(ESS_FILTER);
    customFilterPath = std::string(COMMON_ES9218_PATH);
    customFilterPath.append(ESS_CUSTOM_FILTER);

    mAudioDevicesFactory_V6_0 = ::android::hardware::audio::V6_0::IDevicesFactory::getService();
    if(mAudioDevicesFactory_V6_0 == nullptr) {
        LOG(INFO) << "mAudioDevicesFactory_V6_0 null, trying V5_0";
        mAudioDevicesFactory_V5_0 = ::android::hardware::audio::V5_0::IDevicesFactory::getService();

        if(mAudioDevicesFactory_V5_0 == nullptr) {
            LOG(INFO) << "mAudioDevicesFactory_V5_0 null, trying V4_0";
            mAudioDevicesFactory_V4_0 = ::android::hardware::audio::V4_0::IDevicesFactory::getService();

            if(mAudioDevicesFactory_V4_0 == nullptr) {
                LOG(INFO) << "mAudioDevicesFactory_V4_0 null, aborting";
                return;
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
        default: return; // Should never reach this state
    }

    /* Quad DAC */
    mSupportedFeatures.push_back(Feature::QuadDAC);
    setHifiDacState(getHifiDacState());

    /* Digital Filter */
    if(stat(essFilterPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::DigitalFilter);
        FeatureStates digfilter_fstates;
        digfilter_fstates.states = hidl_vec<KeyValue> {digital_filters};
        mSupportedStates.emplace(Feature::DigitalFilter, digfilter_fstates);
        setFeatureValue(Feature::DigitalFilter, getFeatureValue(Feature::DigitalFilter));
    }

#ifdef PROPRIETARY_AUDIO_MODULE
    /* Sound Presets */
    mSupportedFeatures.push_back(Feature::SoundPreset);
    FeatureStates soundpresets_fstates;
    soundpresets_fstates.states = hidl_vec<KeyValue> {sound_presets};
    mSupportedStates.emplace(Feature::SoundPreset, soundpresets_fstates);
    setFeatureValue(Feature::SoundPreset, getFeatureValue(Feature::SoundPreset));
#endif

    /* Balance Left */
    if(stat(volumeLeftPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::BalanceLeft);
        FeatureStates balanceleft_fstates;
        balanceleft_fstates.range.max = MAX_BALANCE_VALUE;
        balanceleft_fstates.range.min = MIN_BALANCE_VALUE;
        balanceleft_fstates.range.step = 1;
        mSupportedStates.emplace(Feature::BalanceLeft, balanceleft_fstates);
        setFeatureValue(Feature::BalanceLeft, getFeatureValue(Feature::BalanceLeft));
    }

    /* Balance Right */
    if(stat(volumeRightPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::BalanceRight);
        FeatureStates balanceright_fstates;
        balanceright_fstates.range.max = MAX_BALANCE_VALUE;
        balanceright_fstates.range.min = MIN_BALANCE_VALUE;
        balanceright_fstates.range.step = 1;
        mSupportedStates.emplace(Feature::BalanceRight, balanceright_fstates);
        setFeatureValue(Feature::BalanceRight, getFeatureValue(Feature::BalanceRight));
    }

    /* AVC Volume */
    if(stat(avcPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::AVCVolume);
        writeAvcVolumeState(getFeatureValue(Feature::AVCVolume));
    }

    /* Hi-Fi Mode setting */
    if(stat(hifiPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::HifiMode);
        writeHifiModeState(getFeatureValue(Feature::HifiMode));
    }

    /* Custom ESS filter setting */
    if(stat(customFilterPath.c_str(), &buffer) == 0) {
        mSupportedFeatures.push_back(Feature::CustomFilter);
        setCustomFilterShape(getCustomFilterShape());
        setCustomFilterSymmetry(getCustomFilterSymmetry());
        for(int i=0; i < 14; i++)
            setCustomFilterCoeff(i, getCustomFilterCoeff(i));
    }
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
        case Feature::AVCVolume:
            _hidl_cb(getAvcVolumeStates());
            goto end;
        case Feature::HifiMode:
            _hidl_cb(getHifiModeStates());
            goto end;
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

bool DacControl::setAudioHALParameters(KeyValue kv) {
    ::android::hardware::audio::V4_0::Result result_V4_0 = ::android::hardware::audio::V4_0::Result::NOT_SUPPORTED;
    ::android::hardware::audio::V5_0::Result result_V5_0 = ::android::hardware::audio::V5_0::Result::NOT_SUPPORTED;
    ::android::hardware::audio::V6_0::Result result_V6_0 = ::android::hardware::audio::V6_0::Result::NOT_SUPPORTED;

    switch(usedVersion) {
        case AudioVersion::V6_0:
            {
                std::vector<::android::hardware::audio::V6_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V6_0::ParameterValue> parameters_V6_0 =
                        hidl_vec<::android::hardware::audio::V6_0::ParameterValue> {pv_vec};
                result_V6_0 = mAudioDevice_V6_0->setParameters({}, parameters_V6_0);
            }
            break;
        case AudioVersion::V5_0:
            {
                std::vector<::android::hardware::audio::V5_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V5_0::ParameterValue> parameters_V5_0 =
                        hidl_vec<::android::hardware::audio::V5_0::ParameterValue> {pv_vec};
                result_V5_0 = mAudioDevice_V5_0->setParameters({}, parameters_V5_0);
            }
            break;
        case AudioVersion::V4_0:
            {
                std::vector<::android::hardware::audio::V4_0::ParameterValue> pv_vec = {{kv.name, kv.value}};
                hidl_vec<::android::hardware::audio::V4_0::ParameterValue> parameters_V4_0 =
                        hidl_vec<::android::hardware::audio::V4_0::ParameterValue> {pv_vec};
                result_V4_0 = mAudioDevice_V4_0->setParameters({}, parameters_V4_0);
            }
            break;
        default:
            return false;
    }

    if(result_V4_0 == ::android::hardware::audio::V4_0::Result::OK ||
       result_V5_0 == ::android::hardware::audio::V5_0::Result::OK ||
       result_V6_0 == ::android::hardware::audio::V6_0::Result::OK)
        return true;

    // Should not reach this point
    return false;
}

Return<bool> DacControl::setHifiDacState(bool enable) {
    KeyValue kv;
    kv.name = DAC_COMMAND;
    kv.value = enable ? SET_DAC_ON_COMMAND : SET_DAC_OFF_COMMAND;
    return setAudioHALParameters(kv);
}

bool DacControl::setDigitalFilterState(int32_t value) {
    switch(value) {
        case 0: // Short
            set(essFilterPath, 9);
            break;
        case 1: // Sharp
            set(essFilterPath, 4);
            break;
        case 2: // Slow
            set(essFilterPath, 5);
            break;
        case 3: // Custom
            set(essFilterPath, 3);
            break;
        default:
            LOG(ERROR) << "DacControl::setDigitalFilterState: Invalid filter " << value;
            return false;
    }
    property_set(PROPERTY_DIGITAL_FILTER, std::to_string(value).c_str());
    return true;
}

bool DacControl::setVolumeBalance(Feature direction, int32_t value) {
    switch(direction) {
        case Feature::BalanceLeft:
            set(volumeLeftPath, value);
            property_set(PROPERTY_LEFT_BALANCE, std::to_string(value).c_str());
            return true;
        case Feature::BalanceRight:
            set(volumeRightPath, value);
            property_set(PROPERTY_RIGHT_BALANCE, std::to_string(value).c_str());
            return true;
        default:
            return false;
    }
}

Return<bool> DacControl::setFeatureValue(Feature feature, int32_t value) {

    bool rc;

    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), feature) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::setFeatureValue: tried to set value for unsupported Feature...";
        return false;
    }

    KeyValue kv;
    std::string property;
    switch(feature) {
        case Feature::DigitalFilter: {
            return setDigitalFilterState(value);
        }
        case Feature::SoundPreset: {
            kv.name = SET_SOUND_PRESET_COMMAND;
            property = PROPERTY_SOUND_PRESET;
            break;
        }
        case Feature::BalanceLeft:
        case Feature::BalanceRight:
            return setVolumeBalance(feature, value);
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

#ifdef PROPRIETARY_AUDIO_MODULE
    rc = setAudioHALParameters(kv);
#else
    rc = true;
#endif

    if(rc) {
        property_set(property.c_str(), kv.value.c_str());
        return true;
    } else {
        return false;
    }
}

Return<bool> DacControl::getHifiDacState() {
    char value[PROPERTY_VALUE_MAX];
    property_get(PROPERTY_HIFI_DAC_ENABLED, value, PROPERTY_VALUE_HIFI_DAC_DISABLED);
    return (strcmp(value, PROPERTY_VALUE_HIFI_DAC_ENABLED) == 0);
}

Return<int32_t> DacControl::getFeatureValue(Feature feature) {
    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), feature) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::getFeatureValue: tried to set value for unsupported Feature...";
        return -1;
    }

    int32_t property_default_value = 0;
    std::string property;
    char value[PROPERTY_VALUE_MAX];

    switch(feature) {
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
            property = PROPERTY_HIFI_DAC_AVC_VOLUME;
            property_default_value = AVC_VOLUME_DEFAULT;
            break;
        }
        case Feature::HifiMode: {
            property = PROPERTY_HIFI_DAC_MODE;
            property_default_value = HIFI_MODE_DEFAULT;
            break;
        }
        default:
            return false;
    }
    property_get(property.c_str(), value, std::to_string(property_default_value).c_str());
    return std::stoi(value);
}

// Custom filter implementation

Return<int32_t> DacControl::getCustomFilterShape(void) {
    return property_get_int32(PROPERTY_CUSTOM_FILTER_SHAPE, 0);
}

Return<int32_t> DacControl::getCustomFilterSymmetry(void) {
    return property_get_int32(PROPERTY_CUSTOM_FILTER_SYMMETRY, 0);
}

Return<int32_t> DacControl::getCustomFilterCoeff(int32_t coeffIndex) {
    return property_get_int32(PROPERTY_CUSTOM_FILTER_COEFFS.at(coeffIndex).c_str(), 0);
}

std::string DacControl::parseUpdatedCustomFilterData() {
    std::string filter_data;

    /*
    * Let's build the actual string with the custom filter's shape, symmetry and 14 Stage 2 coefficients
    *
    */
    filter_data.append(std::to_string(getCustomFilterShape())).append(",");
    filter_data.append(std::to_string(getCustomFilterSymmetry())).append(",");
    for (int i = 0; i < 14; i++) {
        filter_data.append(std::to_string(getCustomFilterCoeff(i)));
        if(i < 13) /* Last element doesn't need to have a comma appended after it */
            filter_data.append(",");
    }

    return filter_data;
}

Return<bool> DacControl::setCustomFilterShape(int32_t shape) {
    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), Feature::CustomFilter) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::setCustomFilterShape: tried to set custom filter control on unsupported device";
        return false;
    }

    int rc;
    if(shape <= 4)
        rc = property_set(PROPERTY_CUSTOM_FILTER_SHAPE, std::to_string(shape).c_str());
    else /* Filter 5 (counting from 0) is enumerated 6 on es9218.h, so anything after receives +1 as well */
        rc = property_set(PROPERTY_CUSTOM_FILTER_SHAPE, std::to_string(shape + 1).c_str());
    if (rc) {
        LOG(ERROR) << "DacControl::setCustomFilterShape: failed to set property " << PROPERTY_CUSTOM_FILTER_SHAPE << " with error " << rc;
        return false;
    }
    set(customFilterPath, parseUpdatedCustomFilterData());
    return true;
}

Return<bool> DacControl::setCustomFilterSymmetry(int symmetry) {
    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), Feature::CustomFilter) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::setCustomFilterSymmetry: tried to set custom filter control on unsupported device";
        return false;
    }

    int rc = property_set(PROPERTY_CUSTOM_FILTER_SYMMETRY, std::to_string(symmetry).c_str());
    if (rc) {
        LOG(ERROR) << "DacControl::setCustomFilterSymmetry: failed to set property " << PROPERTY_CUSTOM_FILTER_SYMMETRY << " with error " << rc;
        return false;
    }
    set(customFilterPath, parseUpdatedCustomFilterData());
    return true;
}

Return<bool> DacControl::setCustomFilterCoeff(int coeffIndex, int value) {
    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), Feature::CustomFilter) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::setCustomFilterCoeff: tried to set custom filter control on unsupported device";
        return false;
    }

    int rc = property_set(PROPERTY_CUSTOM_FILTER_COEFFS.at(coeffIndex).c_str(), std::to_string(value).c_str());
    if (rc) {
        LOG(ERROR) << "DacControl::setCustomFilterCoeff: failed to set property " << PROPERTY_CUSTOM_FILTER_COEFFS.at(coeffIndex) << " with error " << rc;
        return false;
    }
    set(customFilterPath, parseUpdatedCustomFilterData());
    return true;
}

Return<bool> DacControl::resetCustomFilterCoeffs() {
    int rc;

    if(std::find(mSupportedFeatures.begin(), mSupportedFeatures.end(), Feature::CustomFilter) == mSupportedFeatures.end()) {
        LOG(ERROR) << "DacControl::setCustomFilterCoeff: tried to set custom filter control on unsupported device";
        return false;
    }

    for (int i = 0; i < 14; i++) {
        rc = property_set(PROPERTY_CUSTOM_FILTER_COEFFS.at(i).c_str(), "0");
        if(rc) {
            LOG(ERROR) << "DacControl::resetCustomFilterCoeff: failed to set property " << PROPERTY_CUSTOM_FILTER_COEFFS.at(i) << " with error " << rc;
            return false;
        }
    }
    set(customFilterPath, parseUpdatedCustomFilterData());

    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace control
}  // namespace dac
}  // namespace audio
}  // namespace hardware
}  // namespace lge
}  // namespace vendor
