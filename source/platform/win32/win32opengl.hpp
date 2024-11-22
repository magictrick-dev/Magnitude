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

        virtual bool        bind_to(std::shared_ptr<Window> window);
        virtual bool        unbind();

        bool                context_check_last_error() const;

        static inline thread_local std::shared_ptr<Win32OpenGLRenderContext> context = nullptr;

    protected:
        HGLRC       render_context;

};

#endif
