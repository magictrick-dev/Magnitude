#include <windows.h>
#include <wingdi.h>
#include <platform/win32/win32window.hpp>

Win32Window::
Win32Window(std::string title, i32 width, i32 height)
{

    // Set the properties as provided by the user. Validate.
    // TODO(Chris): I know there's a minimum window size specification that we
    //              should probably enforce here. I should look this up and enforce
    //              a minimum size. Constants are provided for some good default
    //              values as a workaround.
    MAG_ASSERT(width > 0);
    MAG_ASSERT(height > 0);
    MAG_ASSERT(title.length() > 0);
    this->width     = width;
    this->height    = height;
    this->title     = title;

    // Create the display window class and register it.
    WNDCLASSEXW display_window_class    = {0};
    display_window_class.cbSize         = sizeof(display_window_class);
    display_window_class.lpfnWndProc    = &Win32Window::window_procedure;
    display_window_class.hInstance      = GetModuleHandleW(NULL);
    display_window_class.hIcon          = LoadIconA(NULL, IDI_APPLICATION);
    display_window_class.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    display_window_class.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    display_window_class.lpszClassName  = L"displayWindowClass";
    display_window_class.style          = CS_OWNDC;
    RegisterClassExW(&display_window_class);

    // Calculate window size to accomodate frame size + client area.
    RECT client_rect    = {0};
    client_rect.right   = width;
    client_rect.bottom  = height;
    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);
    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    // Attempt to create the window. Normally, I would attempt to rationalize the
    // chaos that is Win32 programming, but alas, I leave the exercise up to the reader.
    HWND window_handle = CreateWindowExW(0, display_window_class.lpszClassName,
            L"Magnitude Rendering Utility", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            window_width, window_height, NULL, NULL, display_window_class.hInstance, this);
    MAG_ASSERT(window_handle != NULL);

    // Establish known properties about the window.
    this->window_handle     = window_handle;
    this->device_context    = GetDC(window_handle);
    this->focused           = true;
    this->minimized         = false;
    this->maximized         = false;
    this->resizable         = true;
    this->visible           = false;
    this->borderless        = false;
    this->was_resized       = false;
    this->was_focused       = false;
    this->was_closed        = false;

    // Now set the actual title.
    SetWindowTextA(window_handle, title.c_str());

}

Win32Window::
~Win32Window()
{

}

void Win32Window::
poll_events()
{

    // Reset our state-change flags.
    this->was_resized = false;
    this->was_focused = false;

    // Pump the message loop.
    MSG current_message = {0};
    while (PeekMessageA(&current_message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&current_message);
        DispatchMessageW(&current_message);
    }

}

void Win32Window::
swap_frames()
{

    SwapBuffers(this->device_context);

}

void Win32Window::
show()
{

    this->set_visibility(true);

}

bool Win32Window::
should_close() const
{
    return this->was_closed;
}

bool Win32Window::
did_size_change() const
{
    return this->was_resized;
}

bool Win32Window::
did_focus_change() const
{
    return this->was_focused;
}

bool Win32Window::
is_borderless() const
{
    return this->borderless;
}

bool Win32Window::
is_focused() const
{
    return this->focused;
}

bool Win32Window::
is_visible() const
{
    return this->visible;
}

bool Win32Window::
is_maximized() const
{
    return this->maximized;
}

bool Win32Window::
is_minimized() const
{
    return this->minimized;
}

i32 Win32Window::
get_height() const
{
    return this->height;
}

i32 Win32Window::
get_width() const
{
    return this->width;
}
 
std::string Win32Window::
get_title() const
{
    return this->title;
}

void Win32Window::
set_borderless(b32 enable, i32 exit_width, i32 exit_height)
{

    if (enable)
    {

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        SetWindowLongPtr(this->window_handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        SetWindowPos(this->window_handle, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);

    }
    
    else
    {

        SetWindowLongPtr(this->window_handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
        SetWindowPos(this->window_handle, NULL, 0, 0, 600, 400, SWP_FRAMECHANGED);   
        this->set_size(exit_width, exit_height);

    }

    this->borderless = enable;

}

void Win32Window::
set_title(const std::string& title)
{

    SetWindowTextA(this->window_handle, title.c_str());

}

void Win32Window::
set_visibility(b32 visible)    
{

    // This function will automatically trigger a window procedure event
    // to update the internal state.
    if (visible)
        ShowWindow(this->window_handle, SW_SHOWNORMAL);
    else
        ShowWindow(this->window_handle, SW_HIDE);

    this->visible = visible;

}

void Win32Window::
set_maximized(b32 maximize)    
{

    // This function will automatically trigger a window procedure event
    // to update the internal state.
    if (maximize) 
    {
        ShowWindow(this->window_handle, SW_MAXIMIZE);
    }
    else 
    {
        ShowWindow(this->window_handle, SW_RESTORE);
    }

}

void Win32Window::
set_size(i32 width, i32 height)
{

    // This function will automatically trigger a window procedure event
    // to update the internal state.
    RECT client_rect    = {0};
    client_rect.right   = width;
    client_rect.bottom  = height;

    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);

    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    SetWindowPos(this->window_handle, NULL, 0, 0,
        window_width, window_height, SWP_NOMOVE);

}

void Win32Window::
set_pixel(i32 x, i32 y, pcolor what)
{

    if (x < 0.0f || x >= this->width || y < 0.0f || y >= this->height) return;
    SetPixel(this->device_context, x, y, (DWORD)what.pack);
    return;

}

pcolor Win32Window::
get_pixel(i32 x, i32 y)
{

    pcolor result = {0};
    if (x < 0.0f || x >= this->width || y < 0.0f || y >= this->height) return result;
    result.pack = (u32)GetPixel(this->device_context, x, y);
    return result;

}


LRESULT CALLBACK Win32Window::
window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{

    Win32Window *self = (Win32Window*)GetWindowLongPtr(window, GWLP_USERDATA);
    LRESULT ret_result = 0;   

    switch (message)
    {

        case WM_CREATE:
        {

            // We have to play pointer hopscotch just to get the Win32Window ptr.
            CREATESTRUCT *creation_context = (CREATESTRUCT*)l_param;
            Win32Window *window_context = (Win32Window*)creation_context->lpCreateParams;
            SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)window_context);

        } break;

        case WM_CLOSE:
        {

            self->was_closed = true;

        } break;

        case WM_SIZE:
        {

            i32 width   = (0xFFFF & l_param);
            i32 height  = (l_param >> 16);

            if (w_param == SIZE_MINIMIZED)
            {
                self->minimized = true;
            }
            else if (w_param == SIZE_MAXIMIZED)
            {
                self->maximized = true;
            }
            else if (w_param == SIZE_RESTORED && self->width == 0 && self->height == 0)
            {
                self->minimized = false;
                self->maximized = false;
            }

            self->width         = width;
            self->height        = height;
            self->was_resized   = true;

        } break;

        case WM_ACTIVATEAPP:
        {

            b32 is_active = (w_param == TRUE);
            self->focused = is_active;
            self->was_focused = true;

        } break;

        default:
        {
            ret_result = DefWindowProcW(window, message, w_param, l_param);
        };

    };

    return ret_result;

}

