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

OpenGLRenderContext::
OpenGLRenderContext()
{

}

OpenGLRenderContext::
~OpenGLRenderContext()
{

}

OpenGLRenderContext& OpenGLRenderContext::
get_render_context()
{

    return *Win32OpenGLRenderContext::context;

}
