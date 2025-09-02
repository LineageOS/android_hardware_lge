/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <fstream>

#include "GloveMode.h"

#ifndef GLOVE_MODE_PATH
#define GLOVE_MODE_PATH "/sys/devices/virtual/input/lge_touch/film_status"
#endif

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

constexpr const char* kFilmStatusPath = GLOVE_MODE_PATH;

bool GloveMode::isSupported() {
    return true;
}

ndk::ScopedAStatus GloveMode::getEnabled(bool* _aidl_return) {

        *_aidl_return = true;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus GloveMode::setEnabled(bool enabled) {

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl