APP_STL := gnustl_static
APP_ABI := arm64-v8a
APP_CPPFLAGS := -frtti -fexceptions
LOCAL_CFLAGS := -mfloat-abi=hard -mfpu=neon -Ofast -mcpu=krait2 -mtune=krait2 -mhard-float
APP_PLATFORM := android-21
NDK_TOOLCHAIN_VERSION:=snapdragonclang3.8

