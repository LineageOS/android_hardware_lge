/*
 * Copyright (C) 2025 Your Name
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "TouchscreenGestureService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <fstream>

#include "TouchscreenGesture.h"

namespace {
struct GestureInfo {
    int32_t id;
    int32_t keycode;
    const char* name;
    int32_t swipe_id; // internal ID used in sysfs
};
}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

const std::string kAvailableGesturePath = "/sys/devices/virtual/input/lge_touch/swipe_available";
const std::string kGesturePath = "/sys/devices/virtual/input/lge_touch/swipe_enable";

const GestureInfo kGestureNodes[] = {
    {0, 247, "Swipe Down", 0},
    {1, 248, "Swipe Up", 1},
    {2, 249, "Swipe Right", 2},
    {3, 250, "Swipe Left", 3},
    {4, 251, "Swipe Bottom Right", 4},
    {5, 252, "Swipe Bottom Left", 5},
};

bool gestureAvailable[6] = {false};

TouchscreenGesture::TouchscreenGesture() {
    std::ifstream file(kAvailableGesturePath);
    std::string line;

    while (std::getline(file, line)) {
        if (line == "0 1") gestureAvailable[0] = true;
        else if (line == "1 1") gestureAvailable[1] = true;
        else if (line == "2 1") gestureAvailable[2] = true;
        else if (line == "3 1") gestureAvailable[3] = true;
        else if (line == "4 1") gestureAvailable[4] = true;
        else if (line == "5 1") gestureAvailable[5] = true;
    }
}

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;

    for (const auto& node : kGestureNodes) {
        if (gestureAvailable[node.id]) {
            gestures.push_back({node.id, node.name, node.keycode});
        }
    }

    *_aidl_return = gestures;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enable) {
    if (gesture.id >= std::size(kGestureNodes)) {
        return ndk::ScopedAStatus::ok();
    }

    const auto& node = kGestureNodes[gesture.id];

    std::ofstream file(kGesturePath);
    if (!file.is_open()) {
        return ndk::ScopedAStatus::ok();
    }

    file << node.swipe_id << " " << (enable ? "1" : "0");
    file.close();

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl