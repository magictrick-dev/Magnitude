#ifndef MAGNITUDE_PLATFORM_WINDOW_H
#define MAGNITUDE_PLATFORM_WINDOW_H
#include <memory>
#include <string>
#include <definitions.hpp>
#include <graphics/color.hpp>
#include <graphics/bitmap.hpp>

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
// The set_bitmap(), set_pixel(), and get_pixel() functions are implemented for
// your personal enrichment. If you want to write to the window, using set_pixel()
// is the easiest way to put a dot down, but it is the slowest. For Windows, the
// WinGDI library does a whole lot of under-the-hood abstractions that make OS
// calls like set_pixel() expensive. If you want get a faster, more authentic
// direct-to-window rendering, use set_bitmap() instead. The idea here is that you
// create a bitmap of the size of the window, render to that and then render that
// to the window. Since your proxy your drawing to memory, it's way faster. The
// OS doesn't care if you render one pixel or one million pixels, a single call
// will always be faster than a million calls.
//
// The API is designed such that you can have multiple windows. This is nice to
// have in case you want to render different images in different windows but the
// trade-off is that it is still single-threaded.
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

        virtual void            set_bitmap(i32 x, i32 y, bitmap_info_header *info, u32 *data) = 0;
        virtual void            set_pixel(i32 x, i32 y, packed_color what) = 0;
        virtual packed_color    get_pixel(i32 x, i32 y) = 0;

};

#endif
