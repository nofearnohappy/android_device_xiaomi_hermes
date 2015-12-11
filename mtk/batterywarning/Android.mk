#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


#
# File permission check daemon
#


#---------------------------------

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := batterywarning.cpp

LOCAL_SHARED_LIBRARIES := libc libcutils libutils libbinder

LOCAL_MODULE := batterywarning

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

#----------------------------------------


