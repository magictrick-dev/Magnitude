#include <windows.h>
#include <iostream>
#include <platform/window.hpp>

LRESULT CALLBACK main_window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

// --- Window Descriptor Implementation ----------------------------------------
//
// All the properties needed to describe our window. Our properties are public
// so that the window procedure can modify them.
//

class Win32Window : public Window
{
    public:
                                        Win32Window(std::string title, i32 width, i32 height);
        inline virtual                 ~Win32Window();

        virtual void                    poll_events();
        virtual void                    swap_frames();
        virtual bool                    should_close() const;
        virtual void                    show();

        inline virtual void             set_size(i32 width, i32 height);
        inline virtual void             set_maximized(b32 maximize);
        inline virtual void             set_visibility(b32 visible);
        inline virtual void             set_title(const std::string& title);
        inline virtual void             set_borderless(b32 enable, i32 ew, i32 eh);

        inline virtual std::string      get_title()         const;
        inline virtual i32              get_width()         const;
        inline virtual i32              get_height()        const;
        inline virtual bool             is_minimized()      const;
        inline virtual bool             is_maximized()      const;
        inline virtual bool             is_visible()        const;
        inline virtual bool             is_borderless()     const;
        inline virtual bool             is_focused()        const;

        inline virtual bool             did_size_change()   const;
        inline virtual bool             did_focus_change()  const;

    public:
        static LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w, LPARAM l);

    public:
        HWND            window_handle;
        HDC             device_context;

        std::string     title;
        i32             width;
        i32             height;
        b32             focused;
        b32             minimized;
        b32             maximized;
        b32             resizable;
        b32             visible;
        b32             borderless;

        b32             was_resized;
        b32             was_focused;
        b32             was_closed;

};

inline Win32Window::
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

inline Win32Window::
~Win32Window()
{

}

inline void Win32Window::
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

inline void Win32Window::
swap_frames()
{

    SwapBuffers(this->device_context);

}

inline void Win32Window::
show()
{

    this->set_visibility(true);

}

inline bool Win32Window::
should_close() const
{
    return this->was_closed;
}

inline bool Win32Window::
did_size_change() const
{
    return this->was_resized;
}

inline bool Win32Window::
did_focus_change() const
{
    return this->was_focused;
}

inline bool Win32Window::
is_borderless() const
{
    return this->borderless;
}

inline bool Win32Window::
is_focused() const
{
    return this->focused;
}

inline bool Win32Window::
is_visible() const
{
    return this->visible;
}

inline bool Win32Window::
is_maximized() const
{
    return this->maximized;
}

inline bool Win32Window::
is_minimized() const
{
    return this->minimized;
}

inline i32 Win32Window::
get_height() const
{
    return this->height;
}

inline i32 Win32Window::
get_width() const
{
    return this->width;
}
 
inline std::string Win32Window::
get_title() const
{
    return this->title;
}

inline void Win32Window::
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

inline void Win32Window::
set_title(const std::string& title)
{

    SetWindowTextA(this->window_handle, title.c_str());

}

inline void Win32Window::
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

inline void Win32Window::
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

inline void Win32Window::
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

// --- Window Base Implementation ----------------------------------------------
//
// The win32 platform implementation of the window.
//

std::shared_ptr<Window> Window::
create(std::string title, i32 width, i32 height)
{

    // NOTE(Chris): Windows will be passed around and used by the renderers
    //              to draw data to them. Therefore, we don't want to prematurely
    //              close the window while its rendering. That's why we use
    //              std::shared_ptr for this; no manual construction of windows.
    Win32Window *window = new Win32Window(title, width, height);
    std::shared_ptr<Window> shared(window);
    return shared;

}
