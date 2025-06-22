/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "GloveModeService"

#include "GloveMode.h"

#include <fstream>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

const std::string kFilmStatusPath = "/sys/devices/virtual/input/lge_touch/film_status";

ndk::ScopedAStatus GloveMode::getEnabled(bool* _aidl_return) {
    std::ifstream file(kFilmStatusPath);
    int enabled;
    file >> enabled;

    if(enabled == 1)
    *_aidl_return = (enabled == 1);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus GloveMode::setEnabled(bool enabled) {
    std::ofstream file(kFilmStatusPath);
    file << (enabled ? "1" : "0");
    return !file.fail() ? ndk::ScopedAStatus::ok() : ndk::ScopedAStatus::fromExceptionCode(EX_TRANSACTION_FAILED);
}

} // namespace touch
} // namespace lineage
} // namespace vendor
} // namespace aidl
