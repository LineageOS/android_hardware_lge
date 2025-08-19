/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <livedisplay/lge/DisplayModes.h>
#include <android-base/logging.h>
#include <fstream>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace implementation {

static constexpr const char* kModePath = "/sys/devices/virtual/panel/img_tune/screen_mode";
static constexpr const char* kDefaultPath = "/data/misc/display/default_screen_mode";

const std::map<int32_t, DisplayModes::ModeInfo> DisplayModes::kModeMap = {
    {0, {"Cinema", "1"}},
    {1, {"Sports", "4"}},
    {2, {"Game", "5"}},
    {3, {"Photos", "2"}},
    {4, {"Web", "3"}},
};

DisplayModes::DisplayModes() {  
    std::ifstream defaultFile(kDefaultPath);
    std::string value;

    defaultFile >> value;
    LOG(DEBUG) << "Default file read result " << value << " fail " << defaultFile.fail();
    if (defaultFile.fail()) {
        return;
    }

    for (const auto& entry : kModeMap) {
        if (value == entry.second.value) {
            mDefaultModeId = entry.first;
            break;
        }
    }

    setDisplayMode(mDefaultModeId, false);
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
ndk::ScopedAStatus DisplayModes::getDisplayModes(std::vector<DisplayMode>* _aidl_return) {
    std::vector<DisplayMode> modes;
    for (const auto& entry : kModeMap) {
        modes.push_back({entry.first, entry.second.name});
    }
    *_aidl_return = modes;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getCurrentDisplayMode(DisplayMode* _aidl_return) {
    int32_t currentModeId = mDefaultModeId;
    std::ifstream modeFile(kModePath);
    std::string value;

    modeFile >> value;
    if (!modeFile.fail()) {
        for (const auto& entry : kModeMap) {
            if (value == entry.second.value) {
                currentModeId = entry.first;
                break;
            }
        }
    }

    *_aidl_return = DisplayMode{(currentModeId, kModeMap.at(currentModeId).name)};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getDefaultDisplayMode(DisplayMode* _aidl_return) {
    *_aidl_return = DisplayMode{(currentModeId, kModeMap.at(currentModeId).name)};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    std::ofstream modeFile(kModePath);
    modeFile << iter->second.value;
    if (modeFile.fail()) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (makeDefault) {
        std::ofstream defaultFile(kDefaultPath);
        defaultFile << iter->second.value;
        if (defaultFile.fail()) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
        mDefaultModeId = iter->first;
    }
    return ndk::ScopedAStatus::ok();
}


}  // namespace implementation
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl

