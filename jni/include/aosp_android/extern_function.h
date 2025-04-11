#ifndef NATIVE_SURFACE_EXTERN_FUNCTION_H
#define NATIVE_SURFACE_EXTERN_FUNCTION_H

#include <iostream>
#include <cstdlib>
#include <android/api-level.h>
#include <android/native_window.h>

#include <aosp_android/utils.h>

struct MDisplayInfo
{
    uint32_t width{0};
    uint32_t height{0};
    uint32_t orientation{0};
};

// method pointer
struct FuncPointer
{
    void *func_createNativeWindow;
    void *func_more_createNativeWindow;
    void *func_getDisplayInfo;
};

class ExternFunction
{
public:
    ExternFunction();

    /**
     * create native surface
     * @param surface_name surface name
     * @param screen_width screen width
     * @param screen_height screen height
     * @param author whether to print the author
     */
    ANativeWindow *
    createNativeWindow(const char *surface_name, uint32_t screen_width, uint32_t screen_height, bool author);

    /**
     * more optional parameters native surface
     * @param surface_name surface name
     * @param screen_width screen width
     * @param screen_height screen height
     * @param format format
     * @param flags flags
     * @param author whether to print the author
     * @return
     */
    ANativeWindow *
    createNativeWindow(const char *surface_name, uint32_t screen_width, uint32_t screen_height, uint32_t format,
                       uint32_t flags, bool author);

    /**
     * get the screen width and height and rotation status
     */
    MDisplayInfo getDisplayInfo();
};

#endif // NATIVE_SURFACE_EXTERN_FUNCTION_H
