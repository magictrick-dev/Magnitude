#include <platform/win32/win32window.hpp>
#include <platform/win32/win32opengl.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <utilities/path.hpp>

void    imgui_initialize_platform(void *platform_handle, void *render_context, i32, i32);
void    imgui_begin_frame();
void    imgui_end_frame();

Win32OpenGLRenderContext::
Win32OpenGLRenderContext(std::shared_ptr<Window> window)
{

    // Cast the pointer to our win32 window.
    std::shared_ptr<Win32Window> win32_window = dynamic_pointer_cast<Win32Window>(window);

    // We will need this.
    HDC device_context  = win32_window->device_context;
    HWND window_handle  = win32_window->window_handle;

    // Set the pixel format descriptor hoopla.
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };


    // This is the goofiest way to do this.
    int pixel_format = ChoosePixelFormat(device_context, &pfd);
    SetPixelFormat(device_context, pixel_format, &pfd);
    this->context_check_last_error();

    // Intiialize OpenGL using GLAD.
    HGLRC opengl_render_context = wglCreateContext(device_context);
    this->render_context = opengl_render_context;
    this->context_check_last_error();

    wglMakeCurrent(device_context, opengl_render_context);
    this->context_check_last_error();

    gladLoadGL();
    gladLoadWGL(device_context);

    // Once the context is made, we can initialize DearImGUI.
    imgui_initialize_platform(window_handle, opengl_render_context, 
            win32_window->width, win32_window->height);

}

Win32OpenGLRenderContext::
~Win32OpenGLRenderContext()
{

    this->unbind();

}

bool Win32OpenGLRenderContext::
bind(std::shared_ptr<Window> window) 
{ 

    // TODO(Chris): We should use GetLastError here to get more information on
    //              why the make current failed. For now, we assume it always works.

    std::shared_ptr<Win32Window> win32_window = dynamic_pointer_cast<Win32Window>(window);
    bool result = wglMakeCurrent(win32_window->device_context, this->render_context);
    return result; 

};

bool Win32OpenGLRenderContext::
unbind() 
{ 

    // TODO(Chris): Will this throw errors if we unmake the context? Test this
    //              and see what happens.

    bool result = wglMakeCurrent(NULL, NULL);
    return true; 

};

void Win32OpenGLRenderContext::
begin_frame()
{
    imgui_begin_frame();
}

void Win32OpenGLRenderContext::
end_frame()
{
    imgui_end_frame();
}

bool Win32OpenGLRenderContext::
context_check_last_error() const
{

    DWORD errorMessageID = GetLastError();
    if (errorMessageID != 0) 
    {
    
        LPSTR messageBuffer = NULL;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&messageBuffer, 0, NULL);
        
        printf("-- OpenGL Context Creation Error: %s\n", messageBuffer);
        LocalFree(messageBuffer);

        return true;

    }

    return false;

}

// --- Dear ImGUI Initialization & Setup --------------------------------------- 
//
// Setting up Dear ImGUI is generally easy, except it isn't so easy if you want
// multiple viewports. A lot of this is glued up code from the examples, so there
// isn't really much we have to do aside ensuring our book-keeping is up to date.
//
// The most important thing to note is that we need to establish our OpenGL context
// first before we can do anything crazy with ImGUI.
//

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// Support function for multi-viewports
// Unlike most other backend combination, we need specific hooks to combine Win32+OpenGL.
// We could in theory decide to support Win32-specific code in OpenGL backend via 
// e.g. an hypothetical ImGui_ImplOpenGL3_InitForRawWin32().
static void Hook_Renderer_CreateWindow(ImGuiViewport* viewport)
{
    assert(viewport->RendererUserData == NULL);

    WGL_WindowData* data = IM_NEW(WGL_WindowData);
    CreateDeviceWGL((HWND)viewport->PlatformHandle, data);
    viewport->RendererUserData = data;
}

static void Hook_Renderer_DestroyWindow(ImGuiViewport* viewport)
{
    if (viewport->RendererUserData != NULL)
    {
        WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData;
        CleanupDeviceWGL((HWND)viewport->PlatformHandle, data);
        IM_DELETE(data);
        viewport->RendererUserData = NULL;
    }
}

static void Hook_Platform_RenderWindow(ImGuiViewport* viewport, void*)
{
    // Activate the platform window DC in the OpenGL rendering context
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        wglMakeCurrent(data->hDC, g_hRC);
}

static void Hook_Renderer_SwapBuffers(ImGuiViewport* viewport, void*)
{
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        ::SwapBuffers(data->hDC);
}


// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };


    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

void 
imgui_initialize_platform(void *platform_handle, void *render_context, i32 width, i32 height)
{

    // Pretty much glued straight from the example code. If it aint broke, don't
    // fix it, I guess.
    g_MainWindow = { GetDC((HWND)platform_handle) };
    g_hRC = (HGLRC)render_context;
    g_Width = width;
    g_Height = height;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg 
    // so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 3.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL((HWND)platform_handle);
    ImGui_ImplOpenGL3_Init();

    // Win32+GL needs specific hooks for viewport, as there are specific things 
    // needed to tie Win32 and GL api.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        IM_ASSERT(platform_io.Renderer_CreateWindow == NULL);
        IM_ASSERT(platform_io.Renderer_DestroyWindow == NULL);
        IM_ASSERT(platform_io.Renderer_SwapBuffers == NULL);
        IM_ASSERT(platform_io.Platform_RenderWindow == NULL);
        platform_io.Renderer_CreateWindow = Hook_Renderer_CreateWindow;
        platform_io.Renderer_DestroyWindow = Hook_Renderer_DestroyWindow;
        platform_io.Renderer_SwapBuffers = Hook_Renderer_SwapBuffers;
        platform_io.Platform_RenderWindow = Hook_Platform_RenderWindow;
    }

    // Just in case our monitor does something weird, we set our monitor
    // work position as main monitor position.
    {
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        for (int i = 0; i < platform_io.Monitors.Size; i++)
        {
            ImGuiPlatformMonitor& mon = platform_io.Monitors[i];
            mon.WorkPos = mon.MainPos;
        }
    }

    // Get the current working directory, set the font.
    Filepath font_path = Filepath::cwd();
    font_path += "./assets/fonts/FiraCode-Medium.ttf";
    font_path.canonicalize();
    MAG_ASSERT(font_path.is_valid_file() && "Unable to locate necessary font.");
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 1;
    io.Fonts->AddFontFromFileTTF(font_path.c_str(), 14, &config);

    // Set the theme.
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.27f, 0.25f, 0.21f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.27f, 0.25f, 0.21f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.42f, 0.36f, 0.29f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.42f, 0.36f, 0.29f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.75f, 0.52f, 0.23f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.75f, 0.52f, 0.23f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.58f, 0.47f, 0.34f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.27f, 0.25f, 0.21f, 1.00f);
    colors[ImGuiCol_TabSelected]            = ImVec4(0.27f, 0.25f, 0.21f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.42f, 0.36f, 0.29f, 1.00f);
    colors[ImGuiCol_TabDimmed]              = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.74f, 0.52f, 0.23f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink]               = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style.WindowRounding        = 4;
    style.TabRounding           = 4;
    style.FrameRounding         = 2;
    style.GrabRounding          = 2;
    style.TabBarOverlineSize    = 0;
    style.FramePadding.y        = 4;

}

void 
imgui_begin_frame()
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

}

void 
imgui_end_frame()
{

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
        wglMakeCurrent(g_MainWindow.hDC, g_hRC);
    }
}
