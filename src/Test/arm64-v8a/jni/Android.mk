LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := TestSimd
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Simd/*.cpp)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS += -llog -ldl -landroid

LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
LOCAL_CPP_FEATURES := rtti exceptions

# 64bit
	APP_ABI	:= arm64-v8a
	LOCAL_CFLAGS  := -Ofast -std=c++11
	TARGET_CFLAGS += -D_NDK_MATH_NO_SOFTFP=1 -DUSE_STDERR_DEBUG -DLOGGING_VERSION_ANDROID -DARCH_ARM64_V8A
	TARGET_CFLAGS  += -Wno-tautological-compare -Wmissing-declarations -Wno-unknown-attributes -Wno-deprecated-declarations
	TARGET_CFLAGS += -Wmissing-declarations  -Wno-deprecated-declarations
	TARGET_CFLAGS += -Wno-multichar -Wno-missing-declarations -Wno-int-to-pointer-cast
	TARGET_LDFLAGS += -Wl,--no-warn-mismatch

CXXFLAGS += -mapcs -fnoomit-frame-pointer
CFLAGS += -mapcs -fnoomit-frame-pointer

include $(BUILD_EXECUTABLE)