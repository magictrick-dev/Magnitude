#ifndef MAGNITUDE_PLATFORM_WIN32_OPENGL_HPP
#define MAGNITUDE_PLATFORM_WIN32_OPENGL_HPP
#include <windows.h>
#include <thread>
#include <glad/glad.h>
#include <glad/glad_wgl.h>
#include <platform/opengl.hpp>

class Win32OpenGLRenderContext : public OpenGLRenderContext
{

    public:
                    Win32OpenGLRenderContext(std::shared_ptr<Window> window);
        virtual    ~Win32OpenGLRenderContext();

        bool        bind(std::shared_ptr<Window> window);
        bool        unbind();
        void        begin_frame();
        void        end_frame();

        bool        context_check_last_error() const;

        static inline thread_local std::shared_ptr<Win32OpenGLRenderContext> context = nullptr;

    protected:
        HGLRC       render_context;

};

#endif
