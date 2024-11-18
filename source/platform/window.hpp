#ifndef MAGNITUDE_PLATFORM_WINDOW_H
#define MAGNITUDE_PLATFORM_WINDOW_H
#include <string>
#include <definitions.hpp>

// --- IWindowDescriptor -------------------------------------------------------
//
// The IWindowDescriptor class allows for subclassing such that internal and
// external APIs can modify the state of the window irrespective of the actual
// window's implementation. For example, any calls to resize are forwarded
// to the window descriptor to trigger. If the internal API encounters events
// that may update the window's properties internally, they're modified inside
// the descriptor.
//
// Generally, the platform implements the functionality since it is OS-specific
// behaviors. For the most part, we don't really want to interact with this
// directly on the front-end and delegate pass-throughs in the Window class.
// This decoupling makes it easier to handle non-user modifications.
//

class IWindowDescriptor
{
    public:
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

        virtual        ~Window();
        void            poll_events();
        void            swap_frames();
        bool            should_close() const;

    public:
        inline i32          get_width() const       { return this->p_width;     }
        inline i32          get_height() const      { return this->p_height;    }
        inline std::string  get_title() const       { return this->p_title;     }

    public:
        inline void         set_title(std::string title)    { this->p_title = title;    }
        inline void         set_width(i32 width)            { this->p_width = width;    }
        inline void         set_height(i32 height)          { this->p_height = height;  }
        inline void         set_size(i32 width, i32 height) { this->p_width = width; this->p_height = height; }

    protected:
                        Window();
                        Window(std::string title, i32 width, i32 height);

        void            initialize_window();
        void            deinitialize_window();

    protected:
        std::unique_ptr<IWindowDescriptor> p_descriptor;

};

#endif
