/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnTouchscreenGesture.h>

#include <map>

using ::aidl::vendor::lineage::touch::Gesture;

namespace {
typedef struct {
    int32_t swipe_id;
    int32_t keycode;
    const char* name;
} GestureInfo;
}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

class TouchscreenGesture : public BnTouchscreenGesture {
public:
    TouchscreenGesture();
    
    ndk::ScopedAStatus getSupportedGestures(std::vector<Gesture>* _aidl_return) override;
    ndk::ScopedAStatus setGestureEnabled(const Gesture& gesture, bool enabled) override;

private:
    std::map<int32_t, GestureInfo> kGestureInfoMap;
};

} // namespace touch
} // namespace lineage
} // namespace vendor
} // namespace aidl
