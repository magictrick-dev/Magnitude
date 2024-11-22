#ifndef MAGNITUDE_PLATFORM_WIN32_OPENGL_HPP
#define MAGNITUDE_PLATFORM_WIN32_OPENGL_HPP
#include <platform/opengl.hpp>
#include <thread>

class Win32OpenGLRenderContext : public OpenGLRenderContext
{

    public:
                    Win32OpenGLRenderContext(std::shared_ptr<Window> window);
        virtual    ~Win32OpenGLRenderContext();

        bool        bind_to(std::shared_ptr<Window> window);
        bool        unbind();

        static inline thread_local std::shared_ptr<Win32OpenGLRenderContext> context = nullptr;

};

#endif
