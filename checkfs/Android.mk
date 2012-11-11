
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)


LOCAL_SRC_FILES:= \
        checkfs.c
        
LOCAL_CFLAGS += -DUSE_EXT4
LOCAL_C_INCLUDES := bootable/recovery
LOCAL_C_INCLUDES += bootable/recovery/libcrecovery
LOCAL_C_INCLUDES += system/extras/ext4_utils
    
LOCAL_MODULE:= checkfs

LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)/sbin
LOCAL_MODULE_TAGS := optional
LOCAL_UNSTRIPPED_PATH := $(TARGET_ROOT_OUT_UNSTRIPPED)


LOCAL_STATIC_LIBRARIES := libcrecovery libext4_utils
LOCAL_STATIC_LIBRARIES += libminui libpixelflinger_static libpng
LOCAL_STATIC_LIBRARIES += libz libstdc++ libcutils libc


include $(BUILD_EXECUTABLE)


