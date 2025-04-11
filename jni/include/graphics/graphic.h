#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>
#include <thread>
#include <chrono>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <android/native_window.h>
#include <Egl/eglext.h>
#include <GLES3/gl3platform.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl32.h>

#include <aosp_android/extern_function.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_android.h>

using namespace std;
using namespace std::chrono_literals;
extern ExternFunction externFunction;
extern MDisplayInfo displayInfo;
extern bool g_Initialized;

bool init_egl(uint32_t _screen_x, uint32_t _screen_y, bool log = false);

bool init_draw(uint32_t _screen_x, uint32_t _screen_y, bool log = false);

bool init_graphic(bool log = false);

bool imgui_init();

void screen_config();

void draw_begin();

void draw_end();

void shut_down();

#endif // GRAPHIC_H
