LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	miravision_jni.cpp
	 
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include \
        $(TOP)/bionic \

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils

ifeq ($(strip $(MTK_AAL_SUPPORT)),yes)
    LOCAL_CFLAGS += -DMTK_AAL_SUPPORT
endif

ifeq ($(strip $(MTK_OD_SUPPORT)),yes)
    LOCAL_CFLAGS += -DMTK_OD_SUPPORT
endif

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libMiraVision_jni
LOCAL_MULTILIB := both

include $(BUILD_SHARED_LIBRARY)
