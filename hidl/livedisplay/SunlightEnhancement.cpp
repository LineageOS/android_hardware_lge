/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "SunlightEnhancementService"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <string>
#include <utils/Errors.h>
#include <fstream>

#include <livedisplay/lge/SunlightEnhancement.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

#define HBM_ENABLED  1
#define HBM_DISABLED 0

static constexpr const char* kHbmEnabledPath = "/sys/devices/virtual/panel/brightness/irc_brighter";

Return<bool> SunlightEnhancement::isEnabled() {
    std::ifstream hbm_status_file(kHbmEnabledPath);
    int result = -1;
    hbm_status_file >> result;
    return !hbm_status_file.fail() && result > 0;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    std::ofstream hbm_enable_file(kHbmEnabledPath);
    hbm_enable_file << (enabled ? HBM_ENABLED : HBM_DISABLED);
    return !hbm_enable_file.fail();
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor