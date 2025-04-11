#include <graphics/graphic.h>
#include <utils/arabic.h>

EGLDisplay display = EGL_NO_DISPLAY;
EGLConfig config;
EGLSurface surface = EGL_NO_SURFACE;
ANativeWindow *native_window;
ExternFunction externFunction;
EGLContext context = EGL_NO_CONTEXT;
MDisplayInfo displayInfo;
uint32_t orientation = 0;
bool g_Initialized = false;

bool init_graphic(bool log)
{
    screen_config();
    orientation = displayInfo.orientation;
    return init_draw(displayInfo.width, displayInfo.height, log);
}

bool init_draw(uint32_t _screen_x, uint32_t _screen_y, bool log)
{
    if (!init_egl(_screen_x, _screen_y, log))
    {
        return false;
    }
    return imgui_init();
}

bool init_egl(uint32_t _screen_x, uint32_t _screen_y, bool log)
{
    native_window = externFunction.createNativeWindow("Ssage",
                                                      _screen_x, _screen_y, true);
    ANativeWindow_acquire(native_window);
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
    {
        printf("eglGetDisplay error=%u\n", glGetError());
        return false;
    }
    if (log)
    {
        printf("eglGetDisplay ok\n");
    }
    if (eglInitialize(display, 0, 0) != EGL_TRUE)
    {
        printf("eglInitialize error=%u\n", glGetError());
        return false;
    }
    if (log)
    {
        printf("eglInitialize ok\n");
    }
    EGLint num_config = 0;
    const EGLint attribList[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_BLUE_SIZE, 5,    //-->delete
        EGL_GREEN_SIZE, 6,   //-->delete
        EGL_RED_SIZE, 5,     //-->delete
        EGL_BUFFER_SIZE, 32, //-->new field
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE};
    const EGLint attrib_list[] = {
        EGL_CONTEXT_CLIENT_VERSION,
        3,
        EGL_NONE};

    if (log)
    {
        printf("num_config=%d\n", num_config);
    }
    if (eglChooseConfig(display, attribList, &config, 1, &num_config) != EGL_TRUE)
    {
        printf("eglChooseConfig  error=%u\n", glGetError());
        return false;
    }
    if (log)
    {
        printf("eglChooseConfig ok\n");
    }
    EGLint egl_format;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &egl_format);
    ANativeWindow_setBuffersGeometry(native_window, 0, 0, egl_format);
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);
    if (context == EGL_NO_CONTEXT)
    {
        printf("eglCreateContext  error = %u\n", glGetError());
        return false;
    }
    if (log)
    {
        printf("eglCreateContext ok\n");
    }
    surface = eglCreateWindowSurface(display, config, native_window, nullptr);
    if (surface == EGL_NO_SURFACE)
    {
        printf("eglCreateWindowSurface  error = %u\n", glGetError());
        return false;
    }
    if (log)
    {
        printf("eglCreateWindowSurface ok\n");
    }
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        printf("eglMakeCurrent  error = %u\n", glGetError());
        return false;
    }
    if (log)
    {
        printf("eglMakeCurrent ok\n");
        printf("createNativeWindow ok\n");
    }
    return true;
}

void screen_config()
{
    displayInfo = externFunction.getDisplayInfo();
}

bool imgui_init()
{
    if (g_Initialized)
    {
        return true;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    ImGui_ImplAndroid_Init(native_window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    static const ImWchar ar_ranges[] = {
        0x0020,
        0x00FF, // Basic Latin + Latin Supplement
        0x0400,
        0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0,
        0x2DFF, // Cyrillic Extended-A
        0xA640,
        0xA69F, // Cyrillic Extended-B
        0xE000,
        0xE226, // icons
        0x2010,
        0x205E, // Punctuations
        0x0600,
        0x06FF, // Arabic
        0xFE00,
        0xFEFF,
        0,
    };

    ImFont *arabic = io.Fonts->AddFontFromMemoryCompressedTTF(font_arabic_data, font_arabic_size, 24.0f, NULL, ar_ranges);
    io.FontDefault = arabic;

    ImGui::GetStyle().ScaleAllSizes(3.0f);

    g_Initialized = true;
    return true;
}

void draw_begin()
{
    screen_config();
    if (orientation != displayInfo.orientation)
    {
        shut_down();
        init_graphic();
        orientation = displayInfo.orientation;
        cout << " width:" << displayInfo.width << " height:" << displayInfo.height << " orientation:"
             << displayInfo.orientation << endl;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame((int32_t)displayInfo.width, (int32_t)displayInfo.height);
    ImGui::NewFrame();
}

void draw_end()
{
    ImGuiIO &io = ImGui::GetIO();
    glViewport(0.0f, 0.0f, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT
    glFlush();
    if (display == EGL_NO_DISPLAY)
    {
        return;
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(display, surface);
}

void shut_down()
{
    if (!g_Initialized)
    {
        return;
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    if (display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    ANativeWindow_release(native_window);
    g_Initialized = false;
}
