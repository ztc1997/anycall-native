
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := anycall
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := anycall.cpp base64.cpp
LOCAL_SHARED_LIBRARIES := libutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
include $(BUILD_EXECUTABLE)
