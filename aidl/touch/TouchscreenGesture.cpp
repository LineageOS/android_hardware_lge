/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <fstream>

#include "TouchscreenGesture.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

const std::string kAvailableGesturePath = "/sys/devices/virtual/input/lge_touch/swipe_available"; 
const std::string kGesturePath = "/sys/devices/virtual/input/lge_touch/swipe_enable"; 
const char* kGestureNames[6] = {
    "Swipe Down",
    "Swipe Up",
    "Swipe Right",
    "Swipe Left",
    "Swipe Bottom Right",
    "Swipe Bottom Left",
};
bool gestureAvailable[6] = {false, false, false, false, false, false };

TouchscreenGesture::TouchscreenGesture() {
    std::ifstream file(kAvailableGesturePath);
    std::string line;
    while(getline(file, line)) {
        if(line == "0 1") {
            gestureAvailable[0] = true;
        } else if(line == "1 1") {
            gestureAvailable[1] = true;
        } else if(line == "2 1") {
            gestureAvailable[2] = true;
        } else if(line == "3 1") {
            gestureAvailable[3] = true;
        } else if(line == "4 1") {
            gestureAvailable[4] = true;
        } else if(line == "5 1") {
            gestureAvailable[5] = true;
        }
    }
    for(int i = 0, j = 0; i < 6; i++) {
        if(gestureAvailable[i]) {
            GestureInfo g = {i, 247+i, kGestureNames[i]};
            kGestureInfoMap.emplace(j, g);
            j++;
        }
    }
}

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;

    for (const auto& entry : kGestureInfoMap) {
        gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
    }
    *_aidl_return = gestures;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    std::ofstream file(kGesturePath);
    std::map<int32_t, GestureInfo>::iterator it;
    it = kGestureInfoMap.find(gesture.id);
    if(it == kGestureInfoMap.end()) {
        return ndk::ScopedAStatus::ok();
    }
    GestureInfo gi = it->second;

    std::string output = std::to_string(gi.swipe_id) + " " + std::to_string(enabled);

    file << output;

    return !file.fail() ? ndk::ScopedAStatus::ok() : ndk::ScopedAStatus::fromExceptionCode(EX_TRANSACTION_FAILED);
}

} // namespace touch
} // namespace lineage
} // namespace vendor
} // namespace aidl
