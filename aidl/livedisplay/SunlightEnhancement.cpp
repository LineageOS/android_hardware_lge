/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/file.h>
#include <android-base/strings.h>
#include <livedisplay/lge/SunlightEnhancement.h>

#include <fstream>

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace lge {

static constexpr const char* kHbmEnabledPath = "/sys/devices/virtual/panel/brightness/irc_brighter";

bool SunlightEnhancement::isSupported() {
    std::fstream file(kHbmEnabledPath, file.in | file.out);
    return file.good();
}

ndk::ScopedAStatus SunlightEnhancement::getEnabled(bool* _aidl_return) {
    std::string tmp;
    int32_t contents = 0;

    if (!ReadFileToString(kHbmEnabledPath, &tmp)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    contents = std::stoi(Trim(tmp));

    *_aidl_return = contents == 1;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SunlightEnhancement::setEnabled(bool enabled) {
    if (!WriteStringToFile(enabled ? "1" : "0", kHbmEnabledPath, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace lge
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
