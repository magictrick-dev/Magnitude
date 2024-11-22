#ifndef MAGNITUDE_PLATFORM_WIN32_WIN32WINDOW_HPP
#define MAGNITUDE_PLATFORM_WIN32_WIN32WINDOW_HPP
#include <windows.h>
#include <platform/window.hpp>

// --- Window Descriptor Implementation ----------------------------------------
//
// All the properties needed to describe our window. Our properties are public
// so that the window procedure can modify them.
//

class Win32Window : public Window
{
    public:
                                Win32Window(std::string title, i32 width, i32 height);
        virtual                ~Win32Window();

        virtual void            poll_events()   override;
        virtual void            swap_frames()   override;
        virtual void            show()          override;
        virtual bool            should_close() const override;

        virtual void            set_size(i32 width, i32 height)             override;
        virtual void            set_maximized(b32 maximize)                 override;
        virtual void            set_visibility(b32 visible)                 override;
        virtual void            set_title(const std::string& title)         override;
        virtual void            set_borderless(b32 enable, i32 ew, i32 eh)  override;

        virtual std::string     get_title()         const override;
        virtual i32             get_width()         const override;
        virtual i32             get_height()        const override;
        virtual bool            is_minimized()      const override;
        virtual bool            is_maximized()      const override;
        virtual bool            is_visible()        const override;
        virtual bool            is_borderless()     const override;
        virtual bool            is_focused()        const override;

        virtual bool            did_size_change()   const override;
        virtual bool            did_focus_change()  const override;

        virtual void            set_bitmap(i32 x, i32 y, bitmap_info_header *info, u32 *data) override;
        virtual void            set_pixel(i32 x, i32 y, packed_color what) override;
        virtual packed_color    get_pixel(i32 x, i32 y) override;

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

#endif
