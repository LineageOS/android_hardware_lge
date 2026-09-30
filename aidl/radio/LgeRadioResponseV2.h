/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vendor/lge/hardware/radio/2.0/ILgeRadioResponseV2.h>

namespace vendor::lge::hardware::radio::implementation {

using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

using ::android::hardware::radio::V1_0::IccIoResult;
using ::android::hardware::radio::V1_0::RadioResponseInfo;
using ::android::hardware::radio::V1_5::DataProfileInfo;
using DataProfileInfo_1_4 = ::android::hardware::radio::V1_4::DataProfileInfo;

using ::vendor::lge::hardware::radio::V2_0::DataRegStateResult;
using ::vendor::lge::hardware::radio::V2_0::DataRegStateResult_1_4;
using ::vendor::lge::hardware::radio::V2_0::LgeCall;
using ::vendor::lge::hardware::radio::V2_0::LgeCardStatus;
using ::vendor::lge::hardware::radio::V2_0::LgeMocaGetMisc;
using ::vendor::lge::hardware::radio::V2_0::LgeModemLoggingData;
using ::vendor::lge::hardware::radio::V2_0::LgeOperatorInfo;
using ::vendor::lge::hardware::radio::V2_0::LgePbmRecordInfo;
using ::vendor::lge::hardware::radio::V2_0::LgePbmRecords;
using ::vendor::lge::hardware::radio::V2_0::LgeSignalStrength;

struct LgeRadioResponseV2 : public V2_0::ILgeRadioResponseV2 {
    // Methods from ::vendor::lge::hardware::radio::V2_0::ILgeRadioResponseV2 follow.
    Return<void> testLgeRadioInterfaceResponse(const RadioResponseInfo& info,
                                               int32_t serial) override {
        return {};
    }
    Return<void> PBMReadRecordResponse(const RadioResponseInfo& info,
                                       const LgePbmRecords& recordInfo) override {
        return {};
    }
    Return<void> PBMWriteRecordResponse(const RadioResponseInfo& info,
                                        const hidl_vec<int32_t>& recordInfo) override {
        return {};
    }
    Return<void> PBMDeleteRecordResponse(const RadioResponseInfo& info,
                                         const hidl_vec<int32_t>& recordInfo) override {
        return {};
    }
    Return<void> PBMGetInitStateResponse(const RadioResponseInfo& info, int32_t initDone) override {
        return {};
    }
    Return<void> PBMGetInfoResponse(const RadioResponseInfo& info,
                                    const LgePbmRecordInfo& recordInfo) override {
        return {};
    }
    Return<void> UIMInternalRequestCmdResponse(const RadioResponseInfo& info, int32_t num,
                                               const hidl_string& data) override {
        return {};
    }
    Return<void> iccSetTransmitBehaviourResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> setCdmaEriVersionResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setCdmaFactoryResetResponse(const RadioResponseInfo& info,
                                             int32_t outData) override {
        return {};
    }
    Return<void> getMipErrorCodeResponse(const RadioResponseInfo& info,
                                         int32_t errorCode) override {
        return {};
    }
    Return<void> cancelManualSearchingRequestResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> setPreviousNetworkSelectionModeManualResponse(
            const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> setRmnetAutoconnectResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> getSearchStatusResponse(const RadioResponseInfo& info, int32_t state) override {
        return {};
    }
    Return<void> getEngineeringModeInfoResponse(const RadioResponseInfo& info,
                                                const hidl_string& modemInfoStr) override {
        return {};
    }
    Return<void> setCSGSelectionManualResponse(const RadioResponseInfo& info,
                                               const hidl_string& session) override {
        return {};
    }
    Return<void> getLteEmmErrorCodeResponse(const RadioResponseInfo& info,
                                            int32_t emmReject) override {
        return {};
    }
    Return<void> loadVolteE911ScanListResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> getVolteE911NetworkTypeResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> exitVolteE911EmergencyModeResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> sendE911CallStateResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setVoiceDomainPrefResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setSrvccCallContextTransferResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> setRssiTestAntConfResponse(const RadioResponseInfo& info, int32_t antConfNum,
                                            int32_t result) override {
        return {};
    }
    Return<void> getRssiTestResponse(const RadioResponseInfo& info,
                                     const hidl_vec<int32_t>& antennaInfo) override {
        return {};
    }
    Return<void> setQcrilResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setMiMoAntennaControlTestResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> setModemInfoResponse(const RadioResponseInfo& info, int32_t data) override {
        return {};
    }
    Return<void> getModemInfoResponse(const RadioResponseInfo& info, int32_t num,
                                      const hidl_string& text) override {
        return {};
    }
    Return<void> getGPRIItemResponse(const RadioResponseInfo& info,
                                     const hidl_string& gpriInfo) override {
        return {};
    }
    Return<void> setGNOSInfoResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setLteBandModeResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setEmergencyResponse(const RadioResponseInfo& info, int32_t ret) override {
        return {};
    }
    Return<void> vssModemResetResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> mocaGetRFParameterResponse(const RadioResponseInfo& info,
                                            const LgeMocaGetMisc& data) override {
        return {};
    }
    Return<void> mocaGetMiscResponse(const RadioResponseInfo& info,
                                     const LgeMocaGetMisc& data) override {
        return {};
    }
    Return<void> mocaAlarmEventResponse(const RadioResponseInfo& info, int8_t result) override {
        return {};
    }
    Return<void> mocaSetLogResponse(const RadioResponseInfo& info, int8_t result) override {
        return {};
    }
    Return<void> mocaGetDataResponse(const RadioResponseInfo& info,
                                     const LgeModemLoggingData& data) override {
        return {};
    }
    Return<void> mocaSetMemResponse(const RadioResponseInfo& info,
                                    const hidl_vec<int32_t>& ret) override {
        return {};
    }
    Return<void> mocaAlarmEventRegResponse(const RadioResponseInfo& info, int32_t ret) override {
        return {};
    }
    Return<void> DMRequestResponse(const RadioResponseInfo& info,
                                   const hidl_vec<int8_t>& data) override {
        return {};
    }
    Return<void> setImsDataFlushEnabledResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> NSRI_SetCaptureMode_requestProcResponse(const RadioResponseInfo& info,
                                                         const hidl_vec<int8_t>& data) override {
        return {};
    }
    Return<void> NSRI_requestProcResponse(const RadioResponseInfo& info,
                                          const hidl_vec<int8_t>& data) override {
        return {};
    }
    Return<void> NSRI_Oem_requestProcResponse(const RadioResponseInfo& info,
                                              const hidl_vec<int8_t>& data) override {
        return {};
    }
    Return<void> setNSRICallInfoTransferResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> sendSarPowerStateResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setImsRegistrationStatusResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> setImsCallStatusResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setScmModeResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> getIMSNetworkInfoResponse(const RadioResponseInfo& info,
                                           const hidl_vec<hidl_string>& data) override {
        return {};
    }
    Return<void> lgeGetSignalStrengthResponse(const RadioResponseInfo& info,
                                              const LgeSignalStrength& signalStrength) override {
        return {};
    }
    Return<void> lgeGetCurrentCallsResponse(const RadioResponseInfo& info,
                                            const hidl_vec<LgeCall>& calls) override {
        return {};
    }
    Return<void> getAvailableNetworksResponse(
            const RadioResponseInfo& info, const hidl_vec<LgeOperatorInfo>& networkInfos) override {
        return {};
    }
    Return<void> getDataRegistrationStateResponse(
            const RadioResponseInfo& info, const DataRegStateResult& dataRegResponse) override {
        return {};
    }
    Return<void> lgeAcknowledgeRequest(int32_t serial) override { return {}; }
    Return<void> setPcasInfofaceResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setLteProcResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setOtasnPdnStateResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setImsCallStateForTuneAwayResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> sendCallDurationResponse(const RadioResponseInfo& info, int32_t result) override {
        return {};
    }
    Return<void> requestWifiIccSimAuthenticationResponse(const RadioResponseInfo& info,
                                                         const IccIoResult& result) override {
        return {};
    }
    Return<void> getWifiIMSIForAppResponse(const RadioResponseInfo& info,
                                           const hidl_string& imsi) override {
        return {};
    }
    Return<void> getWifiIccCardStatusResponse(const RadioResponseInfo& info,
                                              const LgeCardStatus& status) override {
        return {};
    }
    Return<void> sendLgeRequestRawResponse(const RadioResponseInfo& info,
                                           const hidl_vec<int8_t>& data) override {
        return {};
    }
    Return<void> sendLgeRequestStringsResponse(const RadioResponseInfo& info,
                                               const hidl_vec<hidl_string>& data) override {
        return {};
    }
    Return<void> getInitialAttachApnResponse(const RadioResponseInfo& info,
                                             const DataProfileInfo& profile) override {
        return {};
    }
    Return<void> setLge5GEnabledResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setLge5GDisabledResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> getLge5GStatusResponse(const RadioResponseInfo& info, int32_t state) override {
        return {};
    }
    Return<void> setLgeEndcControlResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> notifyImsCallStateResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> changeCallPreferenceResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setLteDataCallTypeResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setTuneawayResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> goDormantResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> reportPdnThrottleIndResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setApnDisableFlagResponse(const RadioResponseInfo& info) override { return {}; }
    Return<void> setApnRoamingDisallowedFlagResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> lgeSetNetworkSelectionModeManualResponse(const RadioResponseInfo& info) override {
        return {};
    }
    Return<void> getDataRegistrationStateResponse_1_3(
            const RadioResponseInfo& info, const DataRegStateResult_1_4& dataRegResponse) override {
        return {};
    }
    Return<void> getInitialAttachApnResponse_1_3(const RadioResponseInfo& info,
                                                 const DataProfileInfo_1_4& profile) override {
        return {};
    }
};

}  // namespace vendor::lge::hardware::radio::implementation
