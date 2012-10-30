$(info $(TARGET_DEVICE) - Look but don't touch )

ifeq ($(TARGET_DEVICE),archos)
$(info Make Device Specific UI Bastard)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := eng
LOCAL_C_INCLUDES += bootable/recovery
LOCAL_SRC_FILES := recovery_ui.c

# should match TARGET_RECOVERY_UI_LIB set in BoardConfig.mk
LOCAL_MODULE := librecovery_ui_archos

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)



#Edify extension functions for doing bootloader updates on archos devices.

LOCAL_MODULE_TAGS := eng
LOCAL_C_INCLUDES += bootable/recovery system/vold
LOCAL_SRC_FILES := recovery_updater.c bootloader.c

# should match TARGET_RECOVERY_UPDATER_LIBS set in BoardConfig.mk
LOCAL_MODULE := librecovery_updater_archos

#include $(BUILD_STATIC_LIBRARY)

endif
