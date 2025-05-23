#ifndef NATIVE_SURFACE_UTILS_H
#define NATIVE_SURFACE_UTILS_H

#include <dlfcn.h>
#include <cerrno>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/system_properties.h>

#include <aosp_android/shm_open_anon.h>
using namespace std;
string exec_native_surface(string command);
void *fdlopen(int fd, int mode);
void *dlblob(const void *blob, size_t len);
int get_android_api_level();
#endif // NATIVE_SURFACE_UTILS_H
