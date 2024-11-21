#ifndef MAGNITUDE_PLATFORM_WINDOW_H
#define MAGNITUDE_PLATFORM_WINDOW_H
#include <string>
#include <definitions.hpp>

// --- Window ------------------------------------------------------------------
//
// A window, as you might imagine, opens a viewport that can be interacted with.
// Windows generally don't do anything special other than maintain and modify the
// state of the window. As such, rendering doesn't occur with windows, rather the
// renderer uses the window to render to.
//
// Since a window is typically platform independent, we delegate the implementation
// of the window to the platform it is defined on.
//

class Window
{
    
    public:
        static std::shared_ptr<Window> create(std::string title, i32 width, i32 height);

        virtual void            poll_events()           = 0;
        virtual void            swap_frames()           = 0;
        virtual bool            should_close() const    = 0;

        virtual void            set_size(i32 width, i32 height)     = 0;
        virtual void            set_position(i32 x, i32 y)          = 0;
        virtual void            set_minimized(b32 minimize)         = 0;
        virtual void            set_maximized(b32 maximize)         = 0;
        virtual void            set_visibility(b32 visible)         = 0;
        virtual void            set_title(const std::string& title) = 0;
        virtual void            set_borderless(b32 enable)          = 0;

        virtual std::string     get_title()     const = 0;
        virtual i32             get_width()     const = 0;
        virtual i32             get_height()    const = 0;
        virtual bool            is_minimized()  const = 0;
        virtual bool            is_maximized()  const = 0;
        virtual bool            is_visible()    const = 0;
        virtual bool            is_borderless() const = 0;
        virtual bool            is_focused()    const = 0;

};

#endif
