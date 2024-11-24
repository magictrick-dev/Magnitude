#include <platform/opengl.hpp>
#include <platform/win32/win32opengl.hpp>

bool OpenGLRenderContext::
create_render_context(std::shared_ptr<Window> window)
{

    if (Win32OpenGLRenderContext::context == nullptr)
    {

        Win32OpenGLRenderContext::context = std::make_shared<Win32OpenGLRenderContext>(window);
        return true;

    }

    return false;

}

bool OpenGLRenderContext::
destroy_render_context()
{

    
    if (Win32OpenGLRenderContext::context != nullptr)
    {

        // Setting it to null will basically yeet the context.
        Win32OpenGLRenderContext::context = nullptr;
        return true;

    }

    return false;

}

OpenGLRenderContext::
OpenGLRenderContext()
{

}

OpenGLRenderContext::
~OpenGLRenderContext()
{

}

bool OpenGLRenderContext::
bind(std::shared_ptr<Window> window)
{

    MAG_ASSERT(Win32OpenGLRenderContext::context != nullptr);
    return Win32OpenGLRenderContext::context->bind(window);

}

bool OpenGLRenderContext::
unbind()
{

    MAG_ASSERT(Win32OpenGLRenderContext::context != nullptr);
    return Win32OpenGLRenderContext::context->unbind();

}

void OpenGLRenderContext::
begin_frame()
{

    MAG_ASSERT(Win32OpenGLRenderContext::context != nullptr);
    Win32OpenGLRenderContext::context->begin_frame();

}

void OpenGLRenderContext::
end_frame()
{

    MAG_ASSERT(Win32OpenGLRenderContext::context != nullptr);
    Win32OpenGLRenderContext::context->end_frame();

}
