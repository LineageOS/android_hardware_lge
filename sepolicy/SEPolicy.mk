#
# Copyright (C) 2022-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

ifeq ($(SEPOLICY_PATH), device/qcom/sepolicy_vndr-legacy-um)
	SEPOLICY_PLATFORM := vndr
else
	SEPOLICY_PLATFORM := legacy-um
endif

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += hardware/lge/sepolicy/private
SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += hardware/lge/sepolicy/public
BOARD_VENDOR_SEPOLICY_DIRS += \
	hardware/lge/sepolicy/vendor \
	hardware/lge/sepolicy/vendor/$(SEPOLICY_PLATFORM)
