#ifndef MAGNITUDE_PLATFORM_WINDOW_H
#define MAGNITUDE_PLATFORM_WINDOW_H
#include <memory>
#include <string>
#include <definitions.hpp>

#define MAGWINDEF_WIDTH     1600
#define MAGWINDEF_HEIGHT    900
#define MAGWINDEF_TITLE     "Magnitude Rendering Utility"

// --- Window ------------------------------------------------------------------
//
// A window, as you might imagine, opens a viewport that can be interacted with.
// Windows generally don't do anything special other than maintain and modify the
// state of the window.
//
// You don't need to trigger anything special for any platform, given the platform
// has a window implementation; just call Window::create() to get back a window
// suitable for the platform you're running on.
//
// The Window interface gives you access to the pixels within the window for
// modification. This is for testing, experimentation, and even for a little bit
// of fun. Per-pixel manipulation is slow, especially if you intend to write a lot
// of pixels to the window. Instead, you should provide a bitmap buffer of all your
// changes so that you make a single call. Writing to memory is *significantly* faster
// than per-pixel addressing. The OS doesn't care if you write one pixel, or all the
// pixels. Therefore, it's way more efficient to feed it a complete bitmap image.
// Either way, power to you. Have fun.
//

class Window
{
    
    public:
        static std::shared_ptr<Window> create(std::string title, i32 width, i32 height);

        virtual void            poll_events()           = 0;
        virtual void            swap_frames()           = 0;
        virtual bool            should_close() const    = 0;
        virtual void            show()                  = 0;

        virtual void            set_size(i32 width, i32 height)     = 0;
        virtual void            set_maximized(b32 maximize)         = 0;
        virtual void            set_visibility(b32 visible)         = 0;
        virtual void            set_title(const std::string& title) = 0;
        virtual void            set_borderless(b32 enable, i32 exit_width, i32 exit_height) = 0;

        virtual std::string     get_title()     const = 0;
        virtual i32             get_width()     const = 0;
        virtual i32             get_height()    const = 0;
        virtual bool            is_minimized()  const = 0;
        virtual bool            is_maximized()  const = 0;
        virtual bool            is_visible()    const = 0;
        virtual bool            is_borderless() const = 0;
        virtual bool            is_focused()    const = 0;

        virtual bool            did_size_change()   const = 0;
        virtual bool            did_focus_change()  const = 0;

};

#endif
