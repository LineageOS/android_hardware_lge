/* Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
#include "LgeRadioIndicationV2.h"

namespace vendor::lge::hardware::radio::implementation {
using ::android::hardware::radio::V1_0::GsmSignalStrength;
using ::android::hardware::radio::V1_4::SignalStrength;

Return<void> LgeRadioIndicationV2::lgeCurrentSignalStrength(
        RadioIndicationType type, const LgeSignalStrength& signalStrength) {
    // Create an AOSP-style GsmSignalStrength and insert needed values.
    GsmSignalStrength newGsmSignalStrength = {.signalStrength = signalStrength.gw.signalStrength,
                                              .bitErrorRate = signalStrength.gw.bitErrorRate,
                                              .timingAdvance = signalStrength.gw.timingAdvance};

    // Create an AOSP-style SignalStrength and insert needed values.
    SignalStrength newSignalStrength = {.gsm = newGsmSignalStrength,
                                        .cdma = signalStrength.cdma,
                                        .evdo = signalStrength.evdo,
                                        .lte = signalStrength.lte,
                                        .tdscdma = signalStrength.tdScdma,
                                        .wcdma = signalStrength.wcdma,
                                        .nr = signalStrength.nr};

    mRadioIndication->currentSignalStrength_1_4(type, newSignalStrength);
    return {};
}

}  // namespace vendor::lge::hardware::radio::implementation
