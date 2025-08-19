/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <livedisplay/lge/ColorEnhancement.h>

#include <android-base/logging.h>
#include <android-base/file.h>
#include <android-base/strings.h>

#include <fstream>

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace lge {

static constexpr const char* kModePath = "/sys/devices/virtual/panel/img_tune/hdr_mode";
static constexpr const char* kDefaultPath = "/data/misc/display/default_hdr_mode";

ColorEnhancement::ColorEnhancement() {
    std::ifstream defaultFile(kDefaultPath);

    defaultFile >> mDefaultColorEnhancement;
    LOG(DEBUG) << "Default file read result " << mDefaultColorEnhancement << " fail " << defaultFile.fail();
    if (defaultFile.fail()) {
        return;
    }

    setEnabled(mDefaultColorEnhancement);
}

bool ColorEnhancement::isSupported() {
    std::ofstream modeFile(kModePath);
    return modeFile.good();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorEnhancement follow.
ndk::ScopedAStatus ColorEnhancement::getEnabled(bool* _aidl_return) {
    std::string tmp;
    int32_t contents = 0;

    if (ReadFileToString(kModePath, &tmp)) {
        contents = std::stoi(Trim(tmp));
    }

    *_aidl_return = contents > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus ColorEnhancement::setEnabled(bool enabled) {
    if (!WriteStringToFile(enabled ? "1" : "0", kModePath, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace lge
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl