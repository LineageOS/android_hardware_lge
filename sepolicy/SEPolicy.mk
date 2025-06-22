#
# Copyright (C) 2022-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

ifneq (,$(filter msm8996 msm8998 sdm845, $(TARGET_BOARD_PLATFORM)))
SEPOLICY_PLATFORM := legacy-um

ifeq (,$(filter sdm845, $(TARGET_BOARD_PLATFORM)))
BOARD_SEPOLICY_M4DEFS += \
    nfc_nq_prop=vendor_nfc_nq_prop \
    qcom_ims_prop=vendor_qcom_ims_prop \
    sensors_prop=vendor_sensors_prop \
    sensors_dbg_prop=vendor_sensors_dbg_prop
endif

else
SEPOLICY_PLATFORM := vndr
endif

ifeq ($(BOARD_LGE_USES_PERSDATA_AS_METADATA),true)
BOARD_SEPOLICY_M4DEFS += \
    persdata_abt_block_device=metadata_block_device
else
BOARD_VENDOR_SEPOLICY_DIRS += \
    hardware/lge/sepolicy/vendor/persdata
endif

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += hardware/lge/sepolicy/private
SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += hardware/lge/sepolicy/public
BOARD_VENDOR_SEPOLICY_DIRS += \
    hardware/lge/sepolicy/vendor \
    hardware/lge/sepolicy/vendor/$(SEPOLICY_PLATFORM)
