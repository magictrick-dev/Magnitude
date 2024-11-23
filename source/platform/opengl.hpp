#ifndef MAGNITUDE_PLATFORM_OPENGL_HPP
#define MAGNITUDE_PLATFORM_OPENGL_HPP
#include <definitions.hpp>
#include <platform/window.hpp>

// --- OpenGL Render Context ---------------------------------------------------
//
// The OpenGL render context allows you to create a valid OpenGL context. There's
// a lot of reasons why the API is designed the way it is, but the major reason is
// that OpenGL defers its implementation to the host platform. In order to even
// create a context (at least on Windows), we need a valid window that we can use
// to create the context. Once a valid context  is made, we don't actually care
// about the window unless we want to bind to it again and render to that window.
//
// Once the context is made, we are able to bind to the window and render to it.
// All OpenGL function pointers are then valid from that point on.
//
// Since OpenGL contexts are relative to the thread, thread local storage is
// used to store the context. If you plan to use multiple windows with multiple
// threads, you will need to create a new context for that thread.
//

class OpenGLRenderContext
{

    public:
        static bool                     create_render_context(std::shared_ptr<Window> window);
        static OpenGLRenderContext&     get_render_context();

        virtual bool    bind_to(std::shared_ptr<Window> window) = 0;
        virtual bool    unbind()        = 0;
        virtual void    begin_frame()   = 0;
        virtual void    end_frame()     = 0;

    protected:
                            OpenGLRenderContext();
        virtual            ~OpenGLRenderContext();

};

#endif
