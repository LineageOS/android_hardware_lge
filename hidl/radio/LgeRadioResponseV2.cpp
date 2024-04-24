/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "LgeRadioResponseV2.h"

namespace vendor::lge::hardware::radio::implementation {

LgeRadioResponseV2::LgeRadioResponseV2(const sp<IRadioResponse>& radioResponse) {
    mRadioResponse = radioResponse;
}

// Methods from ::vendor::lge::hardware::radio::V2_0::ILgeRadioResponseV2 follow.
Return<void> LgeRadioResponseV2::testLgeRadioInterfaceResponse(const RadioResponseInfo& info, int32_t serial) {
    return Void();
}

Return<void> LgeRadioResponseV2::PBMReadRecordResponse(const RadioResponseInfo& info, const LgePbmRecords& recordInfo) {
    return Void();
}

Return<void> LgeRadioResponseV2::PBMWriteRecordResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& recordInfo) {
    return Void();
}

Return<void> LgeRadioResponseV2::PBMDeleteRecordResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& recordInfo) {
    return Void();
}

Return<void> LgeRadioResponseV2::PBMGetInitStateResponse(const RadioResponseInfo& info, int32_t initDone) {
    return Void();
}

Return<void> LgeRadioResponseV2::PBMGetInfoResponse(const RadioResponseInfo& info, const LgePbmRecordInfo& recordInfo) {
    return Void();
}

Return<void> LgeRadioResponseV2::UIMInternalRequestCmdResponse(const RadioResponseInfo& info, int32_t num, const hidl_string& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::iccSetTransmitBehaviourResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setCdmaEriVersionResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setCdmaFactoryResetResponse(const RadioResponseInfo& info, int32_t outData) {
    return Void();
}

Return<void> LgeRadioResponseV2::getMipErrorCodeResponse(const RadioResponseInfo& info, int32_t errorCode) {
    return Void();
}

Return<void> LgeRadioResponseV2::cancelManualSearchingRequestResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setPreviousNetworkSelectionModeManualResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setRmnetAutoconnectResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::getSearchStatusResponse(const RadioResponseInfo& info, int32_t state) {
    return Void();
}

Return<void> LgeRadioResponseV2::getEngineeringModeInfoResponse(const RadioResponseInfo& info, const hidl_string& modemInfoStr) {
    return Void();
}

Return<void> LgeRadioResponseV2::setCSGSelectionManualResponse(const RadioResponseInfo& info, const hidl_string& session) {
    return Void();
}

Return<void> LgeRadioResponseV2::getLteEmmErrorCodeResponse(const RadioResponseInfo& info, int32_t emmReject) {
    return Void();
}

Return<void> LgeRadioResponseV2::loadVolteE911ScanListResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::getVolteE911NetworkTypeResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::exitVolteE911EmergencyModeResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::sendE911CallStateResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setVoiceDomainPrefResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setSrvccCallContextTransferResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setRssiTestAntConfResponse(const RadioResponseInfo& info, int32_t antConfNum, int32_t result) {
    return Void();
}

Return<void> LgeRadioResponseV2::getRssiTestResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& antennaInfo) {
    return Void();
}

Return<void> LgeRadioResponseV2::setQcrilResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setMiMoAntennaControlTestResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setModemInfoResponse(const RadioResponseInfo& info, int32_t data) {
    return Void();
}

Return<void> LgeRadioResponseV2::getModemInfoResponse(const RadioResponseInfo& info, int32_t num, const hidl_string& text) {
    return Void();
}

Return<void> LgeRadioResponseV2::getGPRIItemResponse(const RadioResponseInfo& info, const hidl_string& gpriInfo) {
    return Void();
}

Return<void> LgeRadioResponseV2::setGNOSInfoResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setLteBandModeResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setEmergencyResponse(const RadioResponseInfo& info, int32_t ret) {
    return Void();
}

Return<void> LgeRadioResponseV2::vssModemResetResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaGetRFParameterResponse(const RadioResponseInfo& info, const LgeMocaGetMisc& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaGetMiscResponse(const RadioResponseInfo& info, const LgeMocaGetMisc& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaAlarmEventResponse(const RadioResponseInfo& info, int8_t result) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaSetLogResponse(const RadioResponseInfo& info, int8_t result) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaGetDataResponse(const RadioResponseInfo& info, const LgeModemLoggingData& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaSetMemResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& ret) {
    return Void();
}

Return<void> LgeRadioResponseV2::mocaAlarmEventRegResponse(const RadioResponseInfo& info, int32_t ret) {
    return Void();
}

Return<void> LgeRadioResponseV2::DMRequestResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::setImsDataFlushEnabledResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::NSRI_SetCaptureMode_requestProcResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::NSRI_requestProcResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::NSRI_Oem_requestProcResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::setNSRICallInfoTransferResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::sendSarPowerStateResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setImsRegistrationStatusResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setImsCallStatusResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setScmModeResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::getIMSNetworkInfoResponse(const RadioResponseInfo& info, const hidl_vec<hidl_string>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::lgeGetSignalStrengthResponse(const RadioResponseInfo& info, const LgeSignalStrength& signalStrength) {
    return Void();
}

Return<void> LgeRadioResponseV2::lgeGetCurrentCallsResponse(const RadioResponseInfo& info, const hidl_vec<LgeCall>& calls) {
    return Void();
}

Return<void> LgeRadioResponseV2::getAvailableNetworksResponse(const RadioResponseInfo& info, const hidl_vec<LgeOperatorInfo>& networkInfos) {
    return Void();
}

Return<void> LgeRadioResponseV2::getDataRegistrationStateResponse(const RadioResponseInfo& info, const DataRegStateResult& dataRegResponse) {
    return Void();
}

Return<void> LgeRadioResponseV2::lgeAcknowledgeRequest(int32_t serial) {
    return Void();
}

Return<void> LgeRadioResponseV2::setPcasInfofaceResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setLteProcResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setOtasnPdnStateResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setImsCallStateForTuneAwayResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::sendCallDurationResponse(const RadioResponseInfo& info, int32_t result) {
    return Void();
}

Return<void> LgeRadioResponseV2::requestWifiIccSimAuthenticationResponse(const RadioResponseInfo& info, const IccIoResult& result) {
    return Void();
}

Return<void> LgeRadioResponseV2::getWifiIMSIForAppResponse(const RadioResponseInfo& info, const hidl_string& imsi) {
    return Void();
}

Return<void> LgeRadioResponseV2::getWifiIccCardStatusResponse(const RadioResponseInfo& info, const LgeCardStatus& status) {
    return Void();
}

Return<void> LgeRadioResponseV2::sendLgeRequestRawResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::sendLgeRequestStringsResponse(const RadioResponseInfo& info, const hidl_vec<hidl_string>& data) {
    return Void();
}

Return<void> LgeRadioResponseV2::getInitialAttachApnResponse(const RadioResponseInfo& info, const DataProfileInfo& profile) {
    return Void();
}

Return<void> LgeRadioResponseV2::setLge5GEnabledResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setLge5GDisabledResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::getLge5GStatusResponse(const RadioResponseInfo& info, int32_t state) {
    return Void();
}

Return<void> LgeRadioResponseV2::setLgeEndcControlResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::notifyImsCallStateResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::changeCallPreferenceResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setLteDataCallTypeResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setTuneawayResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::goDormantResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::reportPdnThrottleIndResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setApnDisableFlagResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::setApnRoamingDisallowedFlagResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::lgeSetNetworkSelectionModeManualResponse(const RadioResponseInfo& info) {
    return Void();
}

Return<void> LgeRadioResponseV2::getDataRegistrationStateResponse_1_3(const RadioResponseInfo& info, const DataRegStateResult_1_4& dataRegResponse) {
    return Void();
}

Return<void> LgeRadioResponseV2::getInitialAttachApnResponse_1_3(const RadioResponseInfo& info, const DataProfileInfo_1_4& profile) {
    return Void();
}

}  // namespace vendor::lge::hardware::radio::implementation
