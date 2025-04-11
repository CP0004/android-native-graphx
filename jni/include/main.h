#ifndef MAIN_H
#define MAIN_H

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <sstream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <stdexcept>
#include <random>

#include <utils/vector3.hpp>
#include <utils/Vector2.hpp>
#include <utils/quaternion.hpp>
#include <utils/log.h>
#include <utils/arabic.h>
#include <utils/farsi_type.h>

using namespace std;

static int width = 0;
static int height = 0;
static int orientation = 0;
static bool is_render = false;

static float control_desired_fps = 40.0f;

#include <graphics/graphic.h>
#include <graphics/touch.h>
#include <utils/utils.h>

#endif // MAIN_H