/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnColorEnhancement.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace lge {

class ColorEnhancement : public BnColorEnhancement {
  public:
    ColorEnhancement();
    bool isSupported();

    // Methods from ::vendor::lineage::livedisplay::V2_0::IColorEnhancement follow.
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enabled) override;

  private:
    int32_t mDefaultColorEnhancement;
};

}  // namespace lge
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl