#
# Copyright (C) 2011 Texas Instruments Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# define OMAP_ENHANCEMENT variables
include device/ti/archos/Config.mk

# These two variables are set first, so they can be overridden
# by BoardConfigVendor.mk
BOARD_USES_GENERIC_AUDIO := true
USE_CAMERA_STUB := false

OMAP_ENHANCEMENT_MULTIGPU := true

ENHANCED_DOMX := true
BLTSVILLE_ENHANCEMENT := true
#USE_ITTIAM_AAC := true
# Use the non-open-source parts, if they're present
#-include vendor/ti/archos/BoardConfigVendor.mk

TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := true
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true

BOARD_HAVE_BLUETOOTH := true
TARGET_NO_BOOTLOADER := true

#BOARD_KERNEL_BASE := 0x80000000
# BOARD_KERNEL_CMDLINE

TARGET_NO_RADIOIMAGE := true
TARGET_BOARD_PLATFORM := omap4
#TARGET_BOOTLOADER_BOARD_NAME := archos

BOARD_EGL_CFG := device/ti/archos/egl.cfg

#BOARD_USES_HGL := true
#BOARD_USES_OVERLAY := true
USE_OPENGL_RENDERER := true

# Recovery
#TARGET_RECOVERY_PIXEL_FORMAT := "BGRA_8888"
# device-specific extensions to the updater binary
#TARGET_RELEASETOOLS_EXTENSIONS := device/ti/archos

#BOARD_USES_SECURE_SERVICES := true

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 536870912
#BOARD_SYSTEMIMAGE_PARTITION_SIZE := 16777216
BOARD_USERDATAIMAGE_PARTITION_SIZE := 536870912
BOARD_FLASH_BLOCK_SIZE := 4096

TARGET_SYSTEMIMAGES_USE_EXT4 := true
TARGET_PROVIDES_INIT_RC := true
TARGET_USERIMAGES_SPARSE_EXT_DISABLED := true
TARGET_SYSTEMIMAGES_SPARSE_EXT_DISABLED := true

# Connectivity - Wi-Fi
USES_TI_MAC80211 := true
ifdef USES_TI_MAC80211
BOARD_WPA_SUPPLICANT_DRIVER      := NL80211
WPA_SUPPLICANT_VERSION           := VER_0_8_X_TI
BOARD_HOSTAPD_DRIVER             := NL80211
BOARD_WLAN_DEVICE                := wl12xx_mac80211
BOARD_SOFTAP_DEVICE              := wl12xx_mac80211
WIFI_DRIVER_MODULE_PATH          := "/system/lib/modules/wl12xx_sdio.ko"
WIFI_DRIVER_MODULE_NAME          := "wl12xx_sdio"
WIFI_FIRMWARE_LOADER             := ""
COMMON_GLOBAL_CFLAGS += -DUSES_TI_MAC80211
endif

#Set 32 byte cache line to true
ARCH_ARM_HAVE_32_BYTE_CACHE_LINES := true

BOARD_LIB_DUMPSTATE := libdumpstate.archos

BOARD_VENDOR_TI_GPS_HARDWARE := omap4
BOARD_GPS_LIBRARIES := libgps


# Common device independent definitions
include device/ti/common-open/BoardConfig.mk
