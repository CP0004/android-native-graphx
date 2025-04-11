LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := matrix_64
LOCAL_CFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CPPFLAGS += -fno-exceptions -fpermissive -frtti
LOCAL_LDLIBS += -llog
LOCAL_LDFLAGS += -lEGL -lGLESv2 -lGLESv3 -landroid -llog -lz
LOCAL_CPP_FEATURES := exceptions


LOCAL_C_INCLUDES :=$(LOCAL_PATH)/include
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/imgui
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/imgui/backends
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/aosp_android
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/aosp_android/aosp
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/graphics
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/include/utils
LOCAL_C_INCLUDES +=$(LOCAL_C_INCLUDES:$(LOCAL_PATH)/%:=%)

FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/imgui/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/imgui/backends/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/aosp_android/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/graphics/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/utils/*.c*)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_EXECUTABLE)