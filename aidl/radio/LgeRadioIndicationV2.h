/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/radio/1.4/IRadioIndication.h>
#include <vendor/lge/hardware/radio/2.0/ILgeRadioIndicationV2.h>

namespace vendor::lge::hardware::radio::implementation {

using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

using ::android::hardware::radio::V1_0::RadioIndicationType;
using ::android::hardware::radio::V1_4::IRadioIndication;

using ::vendor::lge::hardware::radio::V2_0::DataPdnThrottleIndInfo;
using ::vendor::lge::hardware::radio::V2_0::ImsPCSCFRestorationVZW;
using ::vendor::lge::hardware::radio::V2_0::LgeDataQosResponse;
using ::vendor::lge::hardware::radio::V2_0::LgeMocaConfigInfo;
using ::vendor::lge::hardware::radio::V2_0::LgeNsriNotice;
using ::vendor::lge::hardware::radio::V2_0::LgeProtocolInfoUnsolInd;
using ::vendor::lge::hardware::radio::V2_0::LgeRpIndResponse;
using ::vendor::lge::hardware::radio::V2_0::LgeSignalStrength;

struct LgeRadioIndicationV2 : public V2_0::ILgeRadioIndicationV2 {
  public:
    explicit LgeRadioIndicationV2(sp<IRadioIndication> indication)
        : mRadioIndication(std::move(indication)) {}

    // Methods from ::vendor::lge::hardware::radio::V2_0::ILgeRadioIndicationV2 follow.
    Return<void> testLgeRadioIndication(int32_t serial) override { return {}; }
    Return<void> racInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> wcdmaNetChanged(RadioIndicationType type, const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> wcdmaNetToKoreaChanged(RadioIndicationType type,
                                        const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> periodicCsgSearch(RadioIndicationType type) override { return {}; }
    Return<void> lgeCipheringInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> lteAcbInfoInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> logRfBandInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> vssMocaMiscNoti(RadioIndicationType type, const LgeMocaConfigInfo& info) override {
        return {};
    }
    Return<void> vssMocaAlaramEvent(RadioIndicationType type,
                                    const LgeMocaConfigInfo& info) override {
        return {};
    }
    Return<void> vssMocaMemLimit(RadioIndicationType type, int32_t limit) override { return {}; }
    Return<void> volteE9111xConnected(RadioIndicationType type,
                                      const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> volteEmergencyCallFailCause(RadioIndicationType type,
                                             const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> volteEmergencyAttachInfo(RadioIndicationType type,
                                          const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> volteLteConnectionStatus(RadioIndicationType type,
                                          const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> voiceCodecIndicator(RadioIndicationType type, int32_t codec) override {
        return {};
    }
    Return<void> lgeLteCaInd(RadioIndicationType type, int32_t lteCaInd) override { return {}; }
    Return<void> protocolInfoInd(RadioIndicationType type,
                                 const LgeProtocolInfoUnsolInd& unsolInfo) override {
        return {};
    }
    Return<void> dataQosChanged(RadioIndicationType type,
                                const LgeDataQosResponse& qosInfo) override {
        return {};
    }
    Return<void> volteE911NetworkType(RadioIndicationType type,
                                      const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> dqslEvent(RadioIndicationType type, int32_t event) override { return {}; }
    Return<void> vzwReservedPcoInfo(RadioIndicationType type,
                                    const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> lteRejectCause(RadioIndicationType type, int32_t rejectCode) override {
        return {};
    }
    Return<void> sib16TimeReceived(RadioIndicationType type, const hidl_string& sib16Time,
                                   int64_t receivedTime) override {
        return {};
    }
    Return<void> lteNetworkInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> modemResetCompleteInd(RadioIndicationType type) override { return {}; }
    Return<void> wcdmaRejectReceived(RadioIndicationType type,
                                     const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> wcdmaAcceptReceived(RadioIndicationType type,
                                     const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> lteEmmReject(RadioIndicationType type, const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> imsPrefStatusInd(RadioIndicationType type,
                                  const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> SsacChangeInfoInd(RadioIndicationType type,
                                   const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> vssNsriNotiMsg(RadioIndicationType type, const LgeNsriNotice& notice) override {
        return {};
    }
    Return<void> resimTimeExpired(RadioIndicationType type) override { return {}; }
    Return<void> lgeCsfbStatusInfo(RadioIndicationType type,
                                   const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> lgeHoStatusInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> lgeNetBandInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> lgeGsmEncrypInfo(RadioIndicationType type,
                                  const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> lgeUnsol(RadioIndicationType type, const LgeRpIndResponse& index) override {
        return {};
    }
    Return<void> lgeRilConnect(RadioIndicationType type) override { return {}; }
    Return<void> lgeCurrentSignalStrength(RadioIndicationType type,
                                          const LgeSignalStrength& signalStrength) override;
    Return<void> rrcStateInd(RadioIndicationType type, int32_t ind) override { return {}; }
    Return<void> dataImsPCSCFResoration(RadioIndicationType type,
                                        const ImsPCSCFRestorationVZW& data) override {
        return {};
    }
    Return<void> onUssdMtk(RadioIndicationType type, int32_t modeType, int32_t ind,
                           const hidl_string& msg) override {
        return {};
    }
    Return<void> volteScmInformation(RadioIndicationType type,
                                     const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> dataPdnThrottleInfo(RadioIndicationType type,
                                     const DataPdnThrottleIndInfo& info) override {
        return {};
    }
    Return<void> newSmsOverIms(RadioIndicationType type, const hidl_string& format,
                               const hidl_vec<int8_t>& pdu) override {
        return {};
    }
    Return<void> newSmsStatusReportOverIms(RadioIndicationType type,
                                           const hidl_vec<int8_t>& pdu) override {
        return {};
    }
    Return<void> onLgeNrDcParamChange(RadioIndicationType type,
                                      const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> nrNetworkInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override {
        return {};
    }
    Return<void> onLgeNrStatusChange(RadioIndicationType type, int32_t state) override {
        return {};
    }
    Return<void> uiccEventNotify(RadioIndicationType type, int32_t slot, const hidl_string& event,
                                 const hidl_string& data) override {
        return {};
    }
    Return<void> smsE911NetworkType(RadioIndicationType type,
                                    const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> callReady(RadioIndicationType type, int32_t data) override { return {}; }
    Return<void> mmtelResponse(RadioIndicationType type, const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> handoffInformation(RadioIndicationType type,
                                    const hidl_vec<int32_t>& data) override {
        return {};
    }
    Return<void> nrRegistrationInfo(RadioIndicationType type,
                                    const hidl_vec<int32_t>& data) override {
        return {};
    }

  private:
    sp<IRadioIndication> mRadioIndication;
};

}  // namespace vendor::lge::hardware::radio::implementation
