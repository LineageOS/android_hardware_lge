/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "LgeRadioIndicationV2.h"

namespace vendor::lge::hardware::radio::implementation {

// Methods from ::vendor::lge::hardware::radio::V2_0::ILgeRadioIndicationV2 follow.
Return<void> LgeRadioIndicationV2::testLgeRadioIndication(int32_t serial) {
    return Void();
}

Return<void> LgeRadioIndicationV2::racInd(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::wcdmaNetChanged(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::wcdmaNetToKoreaChanged(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::periodicCsgSearch(RadioIndicationType type) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeCipheringInd(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lteAcbInfoInd(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::logRfBandInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::vssMocaMiscNoti(RadioIndicationType type, const LgeMocaConfigInfo& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::vssMocaAlaramEvent(RadioIndicationType type, const LgeMocaConfigInfo& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::vssMocaMemLimit(RadioIndicationType type, int32_t limit) {
    return Void();
}

Return<void> LgeRadioIndicationV2::volteE9111xConnected(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::volteEmergencyCallFailCause(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::volteEmergencyAttachInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::volteLteConnectionStatus(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::voiceCodecIndicator(RadioIndicationType type, int32_t codec) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeLteCaInd(RadioIndicationType type, int32_t lteCaInd) {
    return Void();
}

Return<void> LgeRadioIndicationV2::protocolInfoInd(RadioIndicationType type, const LgeProtocolInfoUnsolInd& unsolInfo) {
    return Void();
}

Return<void> LgeRadioIndicationV2::dataQosChanged(RadioIndicationType type, const LgeDataQosResponse& qosInfo) {
    return Void();
}

Return<void> LgeRadioIndicationV2::volteE911NetworkType(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::dqslEvent(RadioIndicationType type, int32_t event) {
    return Void();
}

Return<void> LgeRadioIndicationV2::vzwReservedPcoInfo(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lteRejectCause(RadioIndicationType type, int32_t rejectCode) {
    return Void();
}

Return<void> LgeRadioIndicationV2::sib16TimeReceived(RadioIndicationType type, const hidl_string& sib16Time, int64_t receivedTime) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lteNetworkInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::modemResetCompleteInd(RadioIndicationType type) {
    return Void();
}

Return<void> LgeRadioIndicationV2::wcdmaRejectReceived(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::wcdmaAcceptReceived(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lteEmmReject(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::imsPrefStatusInd(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::SsacChangeInfoInd(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::vssNsriNotiMsg(RadioIndicationType type, const LgeNsriNotice& notice) {
    return Void();
}

Return<void> LgeRadioIndicationV2::resimTimeExpired(RadioIndicationType type) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeCsfbStatusInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeHoStatusInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeNetBandInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeGsmEncrypInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeUnsol(RadioIndicationType type, const LgeRpIndResponse& index) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeRilConnect(RadioIndicationType type) {
    return Void();
}

Return<void> LgeRadioIndicationV2::lgeCurrentSignalStrength(RadioIndicationType type, const LgeSignalStrength& signalStrength) {
    return Void();
}

Return<void> LgeRadioIndicationV2::rrcStateInd(RadioIndicationType type, int32_t ind) {
    return Void();
}

Return<void> LgeRadioIndicationV2::dataImsPCSCFResoration(RadioIndicationType type, const ImsPCSCFRestorationVZW& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::onUssdMtk(RadioIndicationType type, int32_t modeType, int32_t ind, const hidl_string& msg) {
    return Void();
}

Return<void> LgeRadioIndicationV2::volteScmInformation(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::dataPdnThrottleInfo(RadioIndicationType type, const DataPdnThrottleIndInfo& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::newSmsOverIms(RadioIndicationType type, const hidl_string& format, const hidl_vec<int8_t>& pdu) {
    return Void();
}

Return<void> LgeRadioIndicationV2::newSmsStatusReportOverIms(RadioIndicationType type, const hidl_vec<int8_t>& pdu) {
    return Void();
}

Return<void> LgeRadioIndicationV2::onLgeNrDcParamChange(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::nrNetworkInfo(RadioIndicationType type, const hidl_vec<int32_t>& info) {
    return Void();
}

Return<void> LgeRadioIndicationV2::onLgeNrStatusChange(RadioIndicationType type, int32_t state) {
    return Void();
}

Return<void> LgeRadioIndicationV2::uiccEventNotify(RadioIndicationType type, int32_t slot, const hidl_string& event, const hidl_string& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::smsE911NetworkType(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::callReady(RadioIndicationType type, int32_t data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::mmtelResponse(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::handoffInformation(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

Return<void> LgeRadioIndicationV2::nrRegistrationInfo(RadioIndicationType type, const hidl_vec<int32_t>& data) {
    return Void();
}

}  // namespace vendor::lge::hardware::radio::implementation
