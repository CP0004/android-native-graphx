#include <aosp_android/extern_function.h>
#include <aosp_android/aosp/native_surface_9.h>
#include <aosp_android/aosp/native_surface_10.h>
#include <aosp_android/aosp/native_surface_11.h>
#include <aosp_android/aosp/native_surface_12.h>
#include <aosp_android/aosp/native_surface_12_1.h>
#include <aosp_android/aosp/native_surface_13.h>
#include <aosp_android/aosp/native_surface_14.h>

// dynamic library solution
static void *handle;
static FuncPointer funcPointer;

ExternFunction::ExternFunction()
{
    if (!handle)
    {
        printf("android api level:%d\n", get_android_api_level());
        if (get_android_api_level() == 34)
        { // android 14 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_14_64, sizeof(native_surface_14_64));
#else
            handle = dlblob(&native_surface_14_32, sizeof(native_surface_14_32));
#endif
        }
        else if (get_android_api_level() == 33)
        { // android 13 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_13_64, sizeof(native_surface_13_64));
#else
            handle = dlblob(&native_surface_13_32, sizeof(native_surface_13_32));
#endif
        }
        else if (get_android_api_level() == 32)
        { // android 12.1 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_12_1_64, sizeof(native_surface_12_1_64));
#else
            handle = dlblob(&native_surface_12_1_32, sizeof(native_surface_12_1_32));
#endif
        }
        else if (get_android_api_level() == 31)
        { // android 12 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_12_64, sizeof(native_surface_12_64));
#else
            handle = dlblob(&native_surface_12_32, sizeof(native_surface_12_32));
#endif
        }
        else if (get_android_api_level() == 30)
        { // android 11 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_11_64, sizeof(native_surface_11_64));
#else
            handle = dlblob(&native_surface_11_32, sizeof(native_surface_11_32));
#endif
        }
        else if (get_android_api_level() == __ANDROID_API_Q__)
        { // android 10 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_10_64, sizeof(native_surface_10_64));
#else
            handle = dlblob(&native_surface_10_32, sizeof(native_surface_10_32));
#endif
        }
        else if (get_android_api_level() == __ANDROID_API_P__)
        { // android 9 support
            exec_native_surface("su -c settings put global block_untrusted_touches 0");
#ifdef __aarch64__
            handle = dlblob(&native_surface_9_64, sizeof(native_surface_9_64));
#else
            handle = dlblob(&native_surface_9_32, sizeof(native_surface_9_32));
#endif
        }
        else
        {
            printf("Sorry, level:%d Don't Support~\n", get_android_api_level());
            exit(0);
        }
        funcPointer.func_createNativeWindow = dlsym(handle, "_Z18createNativeWindowPKcjjb");
        funcPointer.func_getDisplayInfo = dlsym(handle, "_Z14getDisplayInfov");
    }
}

ANativeWindow *
ExternFunction::createNativeWindow(const char *surface_name, uint32_t screen_width, uint32_t screen_height,
                                   bool author)
{
    return ((ANativeWindow * (*)(const char *, uint32_t, uint32_t, bool))(funcPointer.func_createNativeWindow))(surface_name, screen_width, screen_height, author);
}

ANativeWindow *
ExternFunction::createNativeWindow(const char *surface_name, uint32_t screen_width, uint32_t screen_height,
                                   uint32_t format, uint32_t flags, bool author)
{
    return ((ANativeWindow * (*)(const char *, uint32_t, uint32_t, uint32_t, uint32_t, bool))(funcPointer.func_more_createNativeWindow))(surface_name, screen_width, screen_height, format, flags,
                                                                                                                                         author);
}

MDisplayInfo ExternFunction::getDisplayInfo()
{
    return ((MDisplayInfo(*)())(funcPointer.func_getDisplayInfo))();
}
