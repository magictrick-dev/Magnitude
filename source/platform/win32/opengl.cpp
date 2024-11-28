#include <utilities/logging.hpp>
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

u32 OpenGLRenderContext::
shader_create(u32 type)
{

    return glCreateShader(type);

}

b32 OpenGLRenderContext::
shader_compile(u32 shader, ccptr source)
{


    GLuint shader_identifier = shader;

    glShaderSource(shader_identifier, 1, &source, NULL);
    glCompileShader(shader_identifier);

    // Check compilation status.
    GLint compile_status = GL_FALSE;
    glGetShaderiv(shader_identifier, GL_COMPILE_STATUS, &compile_status);

    // Determine compilation status.
    if (compile_status == GL_FALSE)
    {

        // If the compilation failed, report why to the user through debug output.
        int info_log_length = 0;
        glGetShaderiv(shader_identifier, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0)
        {
            char *message_buffer = (char*)malloc(info_log_length);
            glGetShaderInfoLog(shader_identifier, info_log_length, NULL, message_buffer);
            Logger::log_error(LogFlag_Renderer, "OpenGL shader compilation failed:");
            Logger::log_error(LogFlag_Renderer, "%s", message_buffer);

            free(message_buffer);
        }

        // Exit.
        return false;

    }

    return true;

}

void OpenGLRenderContext::
shader_release(u32 shader)
{

    glDeleteShader(shader);

}

u32 OpenGLRenderContext::
program_create()
{

    return glCreateProgram();

}

b32 OpenGLRenderContext::
program_link(u32 program)
{

    GLuint program_identifier = program;

    glLinkProgram(program_identifier);
 
    GLint program_link_status = GL_FALSE;
    glGetProgramiv(program_identifier, GL_LINK_STATUS, &program_link_status);
    if (program_link_status == GL_FALSE)

    {
        
        int info_log_length = 0;
        glGetProgramiv(program_identifier, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0)
        {
            char *message_buffer = (char*)malloc(info_log_length + 1);
            glGetProgramInfoLog(program_identifier, info_log_length, NULL, message_buffer);
            Logger::log_error(LogFlag_Renderer, "OpenGL program linking failed:");
            Logger::log_error(LogFlag_Renderer, "%s", message_buffer);
            free(message_buffer);
        }

        return false;

    }

    return true;

}

void OpenGLRenderContext::
program_attach(u32 program, u32 shader)
{

    glAttachShader(program, shader);

}

void OpenGLRenderContext::
program_release(u32 program)
{

    glDeleteProgram(program);

}

b32 OpenGLRenderContext::
program_set_uniform_r32(u32 program, ccptr loc, r32 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform1fv(uniform_location, count, (source));
    return true;
}

b32 OpenGLRenderContext::
program_set_uniform_i32(u32 program, ccptr loc, i32 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform1iv(uniform_location, count, (source));
    return true;
}

b32 OpenGLRenderContext::
program_set_uniform_u32(u32 program, ccptr loc, u32 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform1uiv(uniform_location, count, (source));
    return true;
}
