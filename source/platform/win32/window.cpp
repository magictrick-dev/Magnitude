#include <windows.h>
#include <iostream>
#include <platform/window.hpp>

LRESULT CALLBACK main_window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

// --- Window Descriptor Implementation ----------------------------------------
//
// All the properties needed to describe our window. Our properties are public
// so that the window procedure can modify them.
//

class Win32WindowDescriptor : public IWindowDescriptor
{
    public:
        inline virtual void             set_size(i32 width, i32 height)         override;
        inline virtual void             set_position(i32 x, i32 y)              override;
        inline virtual void             set_minimized(b32 minimize)             override;
        inline virtual void             set_maximized(b32 maximize)             override;
        inline virtual void             set_visibility(b32 visible)             override;
        inline virtual void             set_title(const std::string& title)     override;
        inline virtual void             set_borderless(b32 enable)              override;

        inline virtual std::string      get_title()         const override;
        inline virtual i32              get_width()         const override;
        inline virtual i32              get_height()        const override;
        inline virtual bool             is_minimized()      const override;
        inline virtual bool             is_maximized()      const override;
        inline virtual bool             is_visible()        const override;
        inline virtual bool             is_borderless()     const override;
        inline virtual bool             is_focused()        const override;

    public:
        HWND            window_handle;
        HDC             device_context;

        std::string     title;
        i32             width;
        i32             height;
        b32             is_focused;
        b32             is_minimized;
        b32             is_maximized;
        b32             is_resizable;
        b32             is_visible;
        b32             is_borderless;

        b32             was_resized;
        b32             was_focused;
        b32             was_moved;

};

inline bool Win32WindowDescriptor::
is_borderless() const
{
    return this->is_borderless;
}

inline bool Win32WindowDescriptor::
is_focused() const
{
    return this->is_focused;
}

inline bool Win32WindowDescriptor::           
is_visible() const
{
    return this->is_visible;
}

inline bool Win32WindowDescriptor::
is_maximized() const
{
    return this->is_maximized;
}

inline bool Win32WindowDescriptor::
is_minimized() const
{
    return this->is_minimized
}

inline i32 Win32WindowDescriptor::
get_height() const
{
    return this->height;
}

inline i32 Win32WindowDescriptor::
get_width() const
{
    return this->width;
}
 
inline std::string Win32WindowDescriptor::
get_title() const
{
    return this->title;
}

inline void Win32WindowDescriptor::
set_borderless(b32 enable)
{

}

inline void Win32WindowDescriptor::
set_title(const std::string& title)
{

}

inline void Win32WindowDescriptor::
set_visibility(b32 visible)    
{

}

inline void Win32WindowDescriptor::
set_maximized(b32 maximize)    
{

}

inline void Win32WindowDescriptor::
set_minimized(b32 minimize)    
{

}

inline void Win32WindowDescriptor::
set_position(i32 x, i32 y)     
{

}

inline void Win32WindowDescriptor::
set_size(i32 width, i32 height)
{

}

// --- Window Implementation ---------------------------------------------------
//
// The win32 platform implementation of the window.
//

Window::
Window()
{

    this->initialize_window();

}

Window::
Window(std::string title, i32 width, i32 height)
{
    this->initialize_window();
}

Window::
~Window()
{
    this->deinitialize_window();
}

bool Window::
should_close() const
{
    return true;
}

std::shared_ptr<Window> Window::
create(std::string title, i32 width, i32 height)
{

    // NOTE(Chris): Windows will be passed around and used by the renderers
    //              to draw data to them. Therefore, we don't want to prematurely
    //              close the window while its rendering. That's why we use
    //              std::shared_ptr for this; no manual construction of windows.
    Window *window = new Window(title, width, height);
    std::shared_ptr<Window> shared(window);
    return shared;

}

void Window::
initialize_window()
{

    // Create the window procedure.
    WNDCLASSEXW display_window_class    = {0};
    display_window_class.cbSize         = sizeof(display_window_class);
    display_window_class.lpfnWndProc    = &main_window_procedure;
    display_window_class.hInstance      = GetModuleHandleW(NULL);
    display_window_class.hIcon          = LoadIconA(NULL, IDI_APPLICATION);
    display_window_class.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    display_window_class.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    display_window_class.lpszClassName  = L"displayWindowClass";
    display_window_class.style          = CS_OWNDC;

    if (!RegisterClassExW(&display_window_class))
    {
        
        // Generally speaking, this doesn't happen, but if it does, we catch it here.
        // TODO(Chris): It might be worth actually inspecting the reason for the failure
        //              and printing it out. We would want a more robust logging system
        //              for this, so for now we just explode.
        std::cout << "[ Window ] Unable to create window: wndclass registration failed." << std::endl;
        MAG_ASSERT(!"Registration failed, should have worked.");
        return;

    }

    // This will ensure that our client area is the size we specify since Windows
    // for some reason thinks we aren't talking about the actual client area.
    RECT client_rect    = {0};
    client_rect.right   = this->p_width;
    client_rect.bottom  = this->p_height;

    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);

    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    HWND window_handle = CreateWindowExW(0, L"displayWindowClass", L"Temporary Title",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
            NULL, NULL, GetModuleHandleW(NULL), NULL);

    if (window_handle == NULL)
    {
        std::cout << "[ Window ] Unable to create window: handle creation failed." << std::endl;
        MAG_ASSERT(!"Creation failed, should have worked.");
        return;
    }

    //this->p_handle = window_handle;

    // Now that we have the window, we can set the title.
    SetWindowTextA(window_handle, this->p_title.c_str());
    ShowWindow(window_handle, SW_SHOWNORMAL);

}

void Window::
deinitialize_window()
{

}

void Window::
poll_events()
{

}

void Window::
swap_frames()
{

}

// --- Win32 Window Procedure --------------------------------------------------

LRESULT CALLBACK
main_window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{

    LRESULT ret_result = 0;   

    switch (message)
    {

        default:
        {
            ret_result = DefWindowProcW(window, message, w_param, l_param);
        };

    };

    return ret_result;

}
