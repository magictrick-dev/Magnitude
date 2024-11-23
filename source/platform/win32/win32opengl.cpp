#include <platform/win32/win32window.hpp>
#include <platform/win32/win32opengl.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>

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
bind_to(std::shared_ptr<Window> window) 
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

// --- Dear ImGUI --------------------------------------------------------------
//
// Of course, what would a hardware renderer be without Dear ImGUI?
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

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL((HWND)platform_handle);
    ImGui_ImplOpenGL3_Init();

    // Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
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
