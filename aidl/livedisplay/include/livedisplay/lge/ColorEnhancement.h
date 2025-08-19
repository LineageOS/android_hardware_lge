/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/vendor/lineage/livedisplay/BnColorEnhancement.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {
namespace implementation {

class ColorEnhancement : public IColorEnhancement {
  public:
    ColorEnhancement();

    // Methods from ::vendor::lineage::livedisplay::V2_0::IColorEnhancement follow.
    ndk::ScopedAStatus getEnabled() override;
    ndk::ScopedAStatus setEnabled(bool enabled) override;

  private:
    int32_t mDefaultColorEnhancement;
};

}  // namespace implementation
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl