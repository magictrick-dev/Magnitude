#include <platform/win32/win32window.hpp>
#include <platform/win32/win32opengl.hpp>

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
