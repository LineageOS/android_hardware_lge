/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/radio/1.3/IRadioResponse.h>
#include <android/hardware/radio/1.4/IRadioResponse.h>
#include <android/hardware/radio/1.5/IRadioResponse.h>
#include <vendor/lge/hardware/radio/2.0/ILgeRadioResponseV2.h>
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

using ::android::hardware::radio::V1_4::IRadioResponse;
using ::android::hardware::radio::V1_0::RadioResponseInfo;
using ::android::hardware::radio::V1_0::IccIoResult;
using ::android::hardware::radio::V1_5::DataProfileInfo;
using DataProfileInfo_1_4 = ::android::hardware::radio::V1_4::DataProfileInfo;

using ::vendor::lge::hardware::radio::V2_0::LgePbmRecords;
using ::vendor::lge::hardware::radio::V2_0::LgePbmRecordInfo;
using ::vendor::lge::hardware::radio::V2_0::LgeMocaGetMisc;
using ::vendor::lge::hardware::radio::V2_0::LgeModemLoggingData;
using ::vendor::lge::hardware::radio::V2_0::LgeSignalStrength;
using ::vendor::lge::hardware::radio::V2_0::LgeCall;
using ::vendor::lge::hardware::radio::V2_0::LgeOperatorInfo;
using ::vendor::lge::hardware::radio::V2_0::DataRegStateResult;
using ::vendor::lge::hardware::radio::V2_0::DataRegStateResult_1_4;
using ::vendor::lge::hardware::radio::V2_0::LgeCardStatus;

struct LgeRadioResponseV2 : public V2_0::ILgeRadioResponseV2 {
    LgeRadioResponseV2(const sp<IRadioResponse>& radioResponse);
    // Methods from ::vendor::lge::hardware::radio::V2_0::ILgeRadioResponseV2 follow.
    Return<void> testLgeRadioInterfaceResponse(const RadioResponseInfo& info, int32_t serial) override;
    Return<void> PBMReadRecordResponse(const RadioResponseInfo& info, const LgePbmRecords& recordInfo) override;
    Return<void> PBMWriteRecordResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& recordInfo) override;
    Return<void> PBMDeleteRecordResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& recordInfo) override;
    Return<void> PBMGetInitStateResponse(const RadioResponseInfo& info, int32_t initDone) override;
    Return<void> PBMGetInfoResponse(const RadioResponseInfo& info, const LgePbmRecordInfo& recordInfo) override;
    Return<void> UIMInternalRequestCmdResponse(const RadioResponseInfo& info, int32_t num, const hidl_string& data) override;
    Return<void> iccSetTransmitBehaviourResponse(const RadioResponseInfo& info) override;
    Return<void> setCdmaEriVersionResponse(const RadioResponseInfo& info) override;
    Return<void> setCdmaFactoryResetResponse(const RadioResponseInfo& info, int32_t outData) override;
    Return<void> getMipErrorCodeResponse(const RadioResponseInfo& info, int32_t errorCode) override;
    Return<void> cancelManualSearchingRequestResponse(const RadioResponseInfo& info) override;
    Return<void> setPreviousNetworkSelectionModeManualResponse(const RadioResponseInfo& info) override;
    Return<void> setRmnetAutoconnectResponse(const RadioResponseInfo& info) override;
    Return<void> getSearchStatusResponse(const RadioResponseInfo& info, int32_t state) override;
    Return<void> getEngineeringModeInfoResponse(const RadioResponseInfo& info, const hidl_string& modemInfoStr) override;
    Return<void> setCSGSelectionManualResponse(const RadioResponseInfo& info, const hidl_string& session) override;
    Return<void> getLteEmmErrorCodeResponse(const RadioResponseInfo& info, int32_t emmReject) override;
    Return<void> loadVolteE911ScanListResponse(const RadioResponseInfo& info) override;
    Return<void> getVolteE911NetworkTypeResponse(const RadioResponseInfo& info) override;
    Return<void> exitVolteE911EmergencyModeResponse(const RadioResponseInfo& info) override;
    Return<void> sendE911CallStateResponse(const RadioResponseInfo& info) override;
    Return<void> setVoiceDomainPrefResponse(const RadioResponseInfo& info) override;
    Return<void> setSrvccCallContextTransferResponse(const RadioResponseInfo& info) override;
    Return<void> setRssiTestAntConfResponse(const RadioResponseInfo& info, int32_t antConfNum, int32_t result) override;
    Return<void> getRssiTestResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& antennaInfo) override;
    Return<void> setQcrilResponse(const RadioResponseInfo& info) override;
    Return<void> setMiMoAntennaControlTestResponse(const RadioResponseInfo& info) override;
    Return<void> setModemInfoResponse(const RadioResponseInfo& info, int32_t data) override;
    Return<void> getModemInfoResponse(const RadioResponseInfo& info, int32_t num, const hidl_string& text) override;
    Return<void> getGPRIItemResponse(const RadioResponseInfo& info, const hidl_string& gpriInfo) override;
    Return<void> setGNOSInfoResponse(const RadioResponseInfo& info) override;
    Return<void> setLteBandModeResponse(const RadioResponseInfo& info) override;
    Return<void> setEmergencyResponse(const RadioResponseInfo& info, int32_t ret) override;
    Return<void> vssModemResetResponse(const RadioResponseInfo& info) override;
    Return<void> mocaGetRFParameterResponse(const RadioResponseInfo& info, const LgeMocaGetMisc& data) override;
    Return<void> mocaGetMiscResponse(const RadioResponseInfo& info, const LgeMocaGetMisc& data) override;
    Return<void> mocaAlarmEventResponse(const RadioResponseInfo& info, int8_t result) override;
    Return<void> mocaSetLogResponse(const RadioResponseInfo& info, int8_t result) override;
    Return<void> mocaGetDataResponse(const RadioResponseInfo& info, const LgeModemLoggingData& data) override;
    Return<void> mocaSetMemResponse(const RadioResponseInfo& info, const hidl_vec<int32_t>& ret) override;
    Return<void> mocaAlarmEventRegResponse(const RadioResponseInfo& info, int32_t ret) override;
    Return<void> DMRequestResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) override;
    Return<void> setImsDataFlushEnabledResponse(const RadioResponseInfo& info) override;
    Return<void> NSRI_SetCaptureMode_requestProcResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) override;
    Return<void> NSRI_requestProcResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) override;
    Return<void> NSRI_Oem_requestProcResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) override;
    Return<void> setNSRICallInfoTransferResponse(const RadioResponseInfo& info) override;
    Return<void> sendSarPowerStateResponse(const RadioResponseInfo& info) override;
    Return<void> setImsRegistrationStatusResponse(const RadioResponseInfo& info) override;
    Return<void> setImsCallStatusResponse(const RadioResponseInfo& info) override;
    Return<void> setScmModeResponse(const RadioResponseInfo& info) override;
    Return<void> getIMSNetworkInfoResponse(const RadioResponseInfo& info, const hidl_vec<hidl_string>& data) override;
    Return<void> lgeGetSignalStrengthResponse(const RadioResponseInfo& info, const LgeSignalStrength& signalStrength) override;
    Return<void> lgeGetCurrentCallsResponse(const RadioResponseInfo& info, const hidl_vec<LgeCall>& calls) override;
    Return<void> getAvailableNetworksResponse(const RadioResponseInfo& info, const hidl_vec<LgeOperatorInfo>& networkInfos) override;
    Return<void> getDataRegistrationStateResponse(const RadioResponseInfo& info, const DataRegStateResult& dataRegResponse) override;
    Return<void> lgeAcknowledgeRequest(int32_t serial) override;
    Return<void> setPcasInfofaceResponse(const RadioResponseInfo& info) override;
    Return<void> setLteProcResponse(const RadioResponseInfo& info) override;
    Return<void> setOtasnPdnStateResponse(const RadioResponseInfo& info) override;
    Return<void> setImsCallStateForTuneAwayResponse(const RadioResponseInfo& info) override;
    Return<void> sendCallDurationResponse(const RadioResponseInfo& info, int32_t result) override;
    Return<void> requestWifiIccSimAuthenticationResponse(const RadioResponseInfo& info, const IccIoResult& result) override;
    Return<void> getWifiIMSIForAppResponse(const RadioResponseInfo& info, const hidl_string& imsi) override;
    Return<void> getWifiIccCardStatusResponse(const RadioResponseInfo& info, const LgeCardStatus& status) override;
    Return<void> sendLgeRequestRawResponse(const RadioResponseInfo& info, const hidl_vec<int8_t>& data) override;
    Return<void> sendLgeRequestStringsResponse(const RadioResponseInfo& info, const hidl_vec<hidl_string>& data) override;
    Return<void> getInitialAttachApnResponse(const RadioResponseInfo& info, const DataProfileInfo& profile) override;
    Return<void> setLge5GEnabledResponse(const RadioResponseInfo& info) override;
    Return<void> setLge5GDisabledResponse(const RadioResponseInfo& info) override;
    Return<void> getLge5GStatusResponse(const RadioResponseInfo& info, int32_t state) override;
    Return<void> setLgeEndcControlResponse(const RadioResponseInfo& info) override;
    Return<void> notifyImsCallStateResponse(const RadioResponseInfo& info) override;
    Return<void> changeCallPreferenceResponse(const RadioResponseInfo& info) override;
    Return<void> setLteDataCallTypeResponse(const RadioResponseInfo& info) override;
    Return<void> setTuneawayResponse(const RadioResponseInfo& info) override;
    Return<void> goDormantResponse(const RadioResponseInfo& info) override;
    Return<void> reportPdnThrottleIndResponse(const RadioResponseInfo& info) override;
    Return<void> setApnDisableFlagResponse(const RadioResponseInfo& info) override;
    Return<void> setApnRoamingDisallowedFlagResponse(const RadioResponseInfo& info) override;
    Return<void> lgeSetNetworkSelectionModeManualResponse(const RadioResponseInfo& info) override;
    Return<void> getDataRegistrationStateResponse_1_3(const RadioResponseInfo& info, const DataRegStateResult_1_4& dataRegResponse) override;
    Return<void> getInitialAttachApnResponse_1_3(const RadioResponseInfo& info, const DataProfileInfo_1_4& profile) override;

private:
    sp<IRadioResponse> mRadioResponse;
};

}  // namespace vendor::lge::hardware::radio::implementation
