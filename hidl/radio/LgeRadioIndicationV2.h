/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vendor/lge/hardware/radio/2.0/ILgeRadioIndicationV2.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor::lge::hardware::radio::implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

using ::android::hardware::radio::V1_4::IRadioIndication;
using ::android::hardware::radio::V1_0::RadioIndicationType;

using ::vendor::lge::hardware::radio::V2_0::LgeMocaConfigInfo;
using ::vendor::lge::hardware::radio::V2_0::LgeProtocolInfoUnsolInd;
using ::vendor::lge::hardware::radio::V2_0::LgeDataQosResponse;
using ::vendor::lge::hardware::radio::V2_0::LgeNsriNotice;
using ::vendor::lge::hardware::radio::V2_0::LgeRpIndResponse;
using ::vendor::lge::hardware::radio::V2_0::LgeSignalStrength;
using ::vendor::lge::hardware::radio::V2_0::ImsPCSCFRestorationVZW;
using ::vendor::lge::hardware::radio::V2_0::DataPdnThrottleIndInfo;

struct LgeRadioIndicationV2 : public V2_0::ILgeRadioIndicationV2 {
public:
    LgeRadioIndicationV2(const sp<IRadioIndication>& radioIndication);

    // Methods from ::vendor::lge::hardware::radio::V2_0::ILgeRadioIndicationV2 follow.
    Return<void> testLgeRadioIndication(int32_t serial) override;
    Return<void> racInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> wcdmaNetChanged(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> wcdmaNetToKoreaChanged(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> periodicCsgSearch(RadioIndicationType type) override;
    Return<void> lgeCipheringInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> lteAcbInfoInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> logRfBandInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> vssMocaMiscNoti(RadioIndicationType type, const LgeMocaConfigInfo& info) override;
    Return<void> vssMocaAlaramEvent(RadioIndicationType type, const LgeMocaConfigInfo& info) override;
    Return<void> vssMocaMemLimit(RadioIndicationType type, int32_t limit) override;
    Return<void> volteE9111xConnected(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> volteEmergencyCallFailCause(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> volteEmergencyAttachInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> volteLteConnectionStatus(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> voiceCodecIndicator(RadioIndicationType type, int32_t codec) override;
    Return<void> lgeLteCaInd(RadioIndicationType type, int32_t lteCaInd) override;
    Return<void> protocolInfoInd(RadioIndicationType type, const LgeProtocolInfoUnsolInd& unsolInfo) override;
    Return<void> dataQosChanged(RadioIndicationType type, const LgeDataQosResponse& qosInfo) override;
    Return<void> volteE911NetworkType(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> dqslEvent(RadioIndicationType type, int32_t event) override;
    Return<void> vzwReservedPcoInfo(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> lteRejectCause(RadioIndicationType type, int32_t rejectCode) override;
    Return<void> sib16TimeReceived(RadioIndicationType type, const hidl_string& sib16Time, int64_t receivedTime) override;
    Return<void> lteNetworkInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> modemResetCompleteInd(RadioIndicationType type) override;
    Return<void> wcdmaRejectReceived(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> wcdmaAcceptReceived(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> lteEmmReject(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> imsPrefStatusInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> SsacChangeInfoInd(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> vssNsriNotiMsg(RadioIndicationType type, const LgeNsriNotice& notice) override;
    Return<void> resimTimeExpired(RadioIndicationType type) override;
    Return<void> lgeCsfbStatusInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> lgeHoStatusInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> lgeNetBandInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> lgeGsmEncrypInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> lgeUnsol(RadioIndicationType type, const LgeRpIndResponse& index) override;
    Return<void> lgeRilConnect(RadioIndicationType type) override;
    Return<void> lgeCurrentSignalStrength(RadioIndicationType type, const LgeSignalStrength& signalStrength) override;
    Return<void> rrcStateInd(RadioIndicationType type, int32_t ind) override;
    Return<void> dataImsPCSCFResoration(RadioIndicationType type, const ImsPCSCFRestorationVZW& data) override;
    Return<void> onUssdMtk(RadioIndicationType type, int32_t modeType, int32_t ind, const hidl_string& msg) override;
    Return<void> volteScmInformation(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> dataPdnThrottleInfo(RadioIndicationType type, const DataPdnThrottleIndInfo& info) override;
    Return<void> newSmsOverIms(RadioIndicationType type, const hidl_string& format, const hidl_vec<int8_t>& pdu) override;
    Return<void> newSmsStatusReportOverIms(RadioIndicationType type, const hidl_vec<int8_t>& pdu) override;
    Return<void> onLgeNrDcParamChange(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> nrNetworkInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) override;
    Return<void> onLgeNrStatusChange(RadioIndicationType type, int32_t state) override;
    Return<void> uiccEventNotify(RadioIndicationType type, int32_t slot, const hidl_string& event, const hidl_string& data) override;
    Return<void> smsE911NetworkType(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> callReady(RadioIndicationType type, int32_t data) override;
    Return<void> mmtelResponse(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> handoffInformation(RadioIndicationType type, const hidl_vec<int32_t>& data) override;
    Return<void> nrRegistrationInfo(RadioIndicationType type, const hidl_vec<int32_t>& data) override;

private:
    sp<IRadioIndication> mRadioIndication;
};

}  // namespace vendor::lge::hardware::radio::implementation
