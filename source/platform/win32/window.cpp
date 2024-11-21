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

        inline virtual void             set_size(i32 width, i32 height);
        inline virtual void             set_position(i32 x, i32 y);
        inline virtual void             set_minimized(b32 minimize);
        inline virtual void             set_maximized(b32 maximize);
        inline virtual void             set_visibility(b32 visible);
        inline virtual void             set_title(const std::string& title);
        inline virtual void             set_borderless(b32 enable);

        inline virtual std::string      get_title()         const;
        inline virtual i32              get_width()         const;
        inline virtual i32              get_height()        const;
        inline virtual bool             is_minimized()      const;
        inline virtual bool             is_maximized()      const;
        inline virtual bool             is_visible()        const;
        inline virtual bool             is_borderless()     const;
        inline virtual bool             is_focused()        const;

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
        b32             was_moved;

};

inline Win32Window::
Win32Window(std::string title, i32 width, i32 height)
{
    std::cout << "Created a win32 window." << std::endl;
}

inline Win32Window::
~Win32Window()
{
    std::cout << "Destroying a win32 window." << std::endl;
}

inline void Win32Window::
poll_events()
{

}

inline void Win32Window::
swap_frames()
{

}

inline bool Win32Window::
should_close() const
{
    return true;
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
set_borderless(b32 enable)
{

}

inline void Win32Window::
set_title(const std::string& title)
{

}

inline void Win32Window::
set_visibility(b32 visible)    
{

}

inline void Win32Window::
set_maximized(b32 maximize)    
{

}

inline void Win32Window::
set_minimized(b32 minimize)    
{

}

inline void Win32Window::
set_position(i32 x, i32 y)     
{

}

inline void Win32Window::
set_size(i32 width, i32 height)
{

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
