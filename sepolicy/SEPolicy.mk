#
# Copyright (C) 2022-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

ifeq ($(SEPOLICY_PATH), device/qcom/sepolicy-legacy-um)
	SEPOLICY_PLATFORM := legacy-um
else ifeq ($(TARGET_BOARD_PLATFORM),sdm845)
# because SDM845 is different and uses legacy-um definitions in sepolicy_vndr...
	SEPOLICY_PLATFORM := legacy-um
else
	SEPOLICY_PLATFORM := vndr
endif

ifeq ($(BOARD_LGE_USES_PERSDATA_AS_METADATA),true)
	BOARD_SEPOLICY_M4DEFS += persdata_abt_block_device=metadata_block_device
endif

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += hardware/lge/sepolicy/private
SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += hardware/lge/sepolicy/public
BOARD_VENDOR_SEPOLICY_DIRS += \
	hardware/lge/sepolicy/vendor \
	hardware/lge/sepolicy/vendor/$(SEPOLICY_PLATFORM)
