/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORENHANCEMENT_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORENHANCEMENT_H

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/lineage/livedisplay/2.0/IColorEnhancement.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;

class ColorEnhancement : public IColorEnhancement {
  public:
    ColorEnhancement();

    // Methods from ::vendor::lineage::livedisplay::V2_0::IColorEnhancement follow.
    Return<bool> isEnabled() override;
    Return<bool> setEnabled(bool enabled) override;

  private:
    int32_t mDefaultColorEnhancement;

};

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORENHANCEMENT_H
