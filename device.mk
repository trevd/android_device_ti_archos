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


DEVICE_PACKAGE_OVERLAYS := device/ti/archos/overlay

PRODUCT_PACKAGES := \
    libOMX_Core \
    libOMX.TI.DUCATI1.VIDEO.DECODER \
    ti_omap4_ducati_bins 

# Tiler
PRODUCT_PACKAGES += \
    libtimemmgr

#Lib Skia test
PRODUCT_PACKAGES += \
    SkLibTiJpeg_Test \
	
	
ifdef OMAP_ENHANCEMENT_CPCAM
PRODUCT_PACKAGES += \
    libcpcam_jni \
    com.ti.omap.android.cpcam

PRODUCT_COPY_FILES += \
    hardware/ti/omap4xxx/cpcam/com.ti.omap.android.cpcam.xml:system/etc/permissions/com.ti.omap.android.cpcam.xml
endif

PRODUCT_PACKAGES += \
    Camera \
    CameraOMAP4 
    camera_test \
PRODUCT_PACKAGES += \
    power.archos

ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/ti/archos/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

#Need to revisit the fastboot copy files
PRODUCT_COPY_FILES += \
	$(LOCAL_KERNEL):kernel \
	device/ti/archos/ueventd.all.rc:root/ueventd.archosa101sboard.rc \
	device/ti/archos/ueventd.all.rc:root/ueventd.archosa101hboard.rc \
	device/ti/archos/ueventd.all.rc:root/ueventd.archosa80sboard.rc \
	device/ti/archos/ueventd.all.rc:root/ueventd.archosa80hboard.rc \
	device/ti/archos/fstab.archos:root/fstab.archosa101sboard \
	device/ti/archos/init.rc:root/init.rc \
	device/ti/archos/init.rc:root/init.android.rc \
	device/ti/archos/init.recovery.rc:root/init.recovery.rc \
	device/ti/archos/abcbox:root/sbin/abcbox \
	device/ti/archos/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
	device/ti/archos/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	device/ti/archos/twl6030_pwrbutton.kl:system/usr/keylayout/twl6030_pwrbutton.kl \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
	device/ti/common-open/audio/audio_policy.conf:system/etc/audio_policy.conf \
	device/ti/archos/media_profiles.xml:system/etc/media_profiles.xml \
	device/ti/archos/media_codecs.xml:system/etc/media_codecs.xml \
	device/ti/archos/wallpaper_info.xml:data/system/wallpaper_info.xml \
	frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml 

# Archos Specific input device configuration files
PRODUCT_COPY_FILES += \
	device/ti/archos/idc/cpt_i2c_tsp.idc:system/usr/idc/cpt_i2c_tsp.idc \
	device/ti/archos/idc/cypress-tma340.idc:system/usr/idc/cypress-tma340.idc \
	device/ti/archos/idc/pixcir_i2c_tsp.idc:system/usr/idc/pixcir_i2c_tsp.idc \
	device/ti/archos/idc/qwerty2.idc:system/usr/idc/qwerty2.idc \
	device/ti/archos/idc/qwerty.idc:system/usr/idc/qwerty.idc \
	device/ti/archos/idc/tr16c0_i2c_tsp.idc:system/usr/idc/tr16c0_i2c_tsp.idc
	
# to mount the external storage (sdcard)
PRODUCT_COPY_FILES += \
	device/ti/archos/recovery.fstab:root/sbin/recovery.fstab \
        device/ti/archos/vold.fstab:system/etc/vold.fstab

PRODUCT_PACKAGES += \
	lights.archos

#Remove this as it freezes at boot. Will re-enable once fixed
PRODUCT_PACKAGES += \
	sensors.archos \
	sensor.test

PRODUCT_PACKAGES += \
       boardidentity \
       libboardidentity \
       libboard_idJNI \
       Board_id

PRODUCT_PACKAGES += \
	com.android.future.usb.accessory

# Live Wallpapers
PRODUCT_PACKAGES += \
        librs_jni \
        LiveWallpapers \
        LiveWallpapersPicker \
        MagicSmokeWallpapers \
        VisualizationWallpapers \


PRODUCT_PACKAGES += \
	VideoEditorGoogle

PRODUCT_PROPERTY_OVERRIDES := \
	hwui.render_dirty_regions=false

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	ro.secure=0 \
	#persist.sys.usb.config=

PRODUCT_CHARACTERISTICS := nosdcard,tablet

PRODUCT_PROPERTY_OVERRIDES += \
	ro.opengles.version=131072

PRODUCT_PROPERTY_OVERRIDES += \
	ro.sf.lcd_density=160

PRODUCT_PROPERTY_OVERRIDES += \
	persist.hwc.mirroring.region=0:0:1280:720

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
	librs_jni \
	com.android.future.usb.accessory

# WI-Fi
PRODUCT_PACKAGES += \
	dhcpcd.conf \
	hostapd.conf \
	wifical.sh \
	TQS_D_1.7.ini \
	TQS_D_1.7_127x.ini \
	crda \
	regulatory.bin \
	calibrator

PRODUCT_PROPERTY_OVERRIDES += \
	wifi.interface=wlan0

# Filesystem management tools
PRODUCT_PACKAGES += \
	make_ext4fs \
	setup_fs

# Audio HAL module
PRODUCT_PACKAGES += audio.primary.omap4
PRODUCT_PACKAGES += audio.hdmi.omap4

# Audioout libs
PRODUCT_PACKAGES += libaudioutils

# for bugmailer
PRODUCT_PACKAGES += send_bug
PRODUCT_COPY_FILES += \
	system/extras/bugmailer/bugmailer.sh:system/bin/bugmailer.sh \
	system/extras/bugmailer/send_bug:system/bin/send_bug

# tinyalsa utils
PRODUCT_PACKAGES += \
	tinymix \
	tinyplay \
	tinycap

# These are the hardware-specific features
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.barometer.xml:system/etc/permissions/android.hardware.sensor.barometer.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml \
    device/ti/archos/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml 

# BlueZ a2dp Audio HAL module
PRODUCT_PACKAGES += audio.a2dp.default

# BlueZ test tools
PRODUCT_PACKAGES += \
	hciconfig \
	hcitool

# SMC components for secure services like crypto, secure storage
PRODUCT_PACKAGES += \
        smc_pa.ift \
        smc_normal_world_android_cfg.ini \
        libsmapi.so \
        libtf_crypto_sst.so \
        libtfsw_jce_provider.so \
        tfsw_jce_provider.jar \
        tfctrl

# Enable AAC 5.1 decode (decoder)
PRODUCT_PROPERTY_OVERRIDES += \
	media.aac_51_output_enabled=true

PRODUCT_PACKAGES += \
	archos_hdcp_keys

# Expirimental Apps
PRODUCT_PACKAGES += \
	DreamTheater \
	VideoChatCameraTestApp

$(call inherit-product, frameworks/native/build/tablet-dalvik-heap.mk)
$(call inherit-product, hardware/ti/omap4xxx/omap4.mk)
#$(call inherit-product-if-exists, vendor/ti/archos/device-vendor.mk)
$(call inherit-product-if-exists, hardware/ti/wpan/ti-wpan-products.mk)
$(call inherit-product-if-exists, device/ti/proprietary-open/omap4/ti-omap4-vendor.mk)
$(call inherit-product-if-exists, device/ti/proprietary-open/wl12xx/wlan/wl12xx-wlan-fw-products.mk)
$(call inherit-product-if-exists, device/ti/common-open/s3d/s3d-products.mk)
$(call inherit-product-if-exists, device/ti/proprietary-open/omap4/ducati-archos.mk)
