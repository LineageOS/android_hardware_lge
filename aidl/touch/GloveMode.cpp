/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <fstream>

#include "GloveMode.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

constexpr const char* kFilmStatusPath = "/sys/devices/virtual/input/lge_touch/film_status";

bool GloveMode::isSupported() {
    std::ifstream file(kFilmStatusPath);
    if (file.is_open()) {
        int enabled = 0;
        file >> enabled;
        if (enabled == 1) return true;
        file.close();
    }
    return false;
}

ndk::ScopedAStatus GloveMode::getEnabled(bool* _aidl_return) {
    std::ifstream file(kFilmStatusPath);
   
    int enabled = 0;
    file >> enabled;

    if (file.fail()) {
        *_aidl_return = false;
    } else {
        *_aidl_return = (enabled == 1);
    }
    
    file.close();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus GloveMode::setEnabled(bool enabled) {
    std::ofstream file(kFilmStatusPath);
      file << (enabled ? "1" : "0");
    file.close();

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
