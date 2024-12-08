#include <renderer/pipeline.hpp>
#include <glad/glad.h>

// --- Point Pipeline ----------------------------------------------------------
//
// Basically, it just renders a single point to the screen.
//

ccptr point_vertex_shader = R"(#version 430 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in float a_radius;
out float v_radius;

uniform mat4 projection;
uniform mat4 view;

void main()
{

    v_radius = a_radius;
    gl_Position = projection * view * vec4(a_position, 1.0f);

}

)";

ccptr point_fragment_shader = R"(#version 430 core
in float v_radius;
out vec4 v_fragment_color;

void main()
{

    v_fragment_color = vec4(1.0f, 0.5f, 0.5f, 1.0f);

}
)";

void RendererPipeline::
point_pipeline(vec3 position, r32 radius)
{

    RendererPipeline::point_objects.push_back({
        .position = position, 
        .radius = radius,
    });

    if (RendererPipeline::point_objects.size() >= PIPELINE_POINTS_FLUSH_LIMIT)
        RendererPipeline::flush_point_pipeline();

}

void RendererPipeline::
flush_point_pipeline()
{

    static bool     point_program_initialized = false;
    static u32      point_program_identifier = 0;   
    static u32      point_program_vao = 0;
    static u32      point_program_vbo = 0;

    if (point_program_initialized == false)
    {

        // Create the program.
        u32 vertex_shader = OpenGLRenderContext::shader_create(GL_VERTEX_SHADER);
        u32 fragment_shader = OpenGLRenderContext::shader_create(GL_FRAGMENT_SHADER);
        MAG_ASSERT(OpenGLRenderContext::shader_compile(vertex_shader, 
                    point_vertex_shader));
        MAG_ASSERT(OpenGLRenderContext::shader_compile(fragment_shader, 
                    point_fragment_shader));

        point_program_identifier = OpenGLRenderContext::program_create();
        OpenGLRenderContext::program_attach(point_program_identifier, vertex_shader);
        OpenGLRenderContext::program_attach(point_program_identifier, fragment_shader);
        MAG_ASSERT(OpenGLRenderContext::program_link(point_program_identifier));
        OpenGLRenderContext::shader_release(vertex_shader);
        OpenGLRenderContext::shader_release(fragment_shader);

        // Generate the buffers, set the buffer layout.
        glGenVertexArrays(1, &point_program_vao);
        glBindVertexArray(point_program_vao);

        glGenBuffers(1, &point_program_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, point_program_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point_object)*PIPELINE_POINTS_FLUSH_LIMIT,
                RendererPipeline::point_objects.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(NULL);
        point_program_initialized = true;
        
    }

    

}

// --- Line Pipeline -----------------------------------------------------------
//
// Lines pipelines render lines to the screen.
//

void RendererPipeline::
line_pipeline(vec3 a, vec3 b, r32 width)
{

    RendererPipeline::line_objects.push_back({
        .a = a, 
        .b = b,
        .width = width,
    });

    if (RendererPipeline::line_objects.size() >= PIPELINE_LINE_FLUSH_LIMIT)
        RendererPipeline::flush_line_pipeline();

}

void RendererPipeline::
flush_line_pipeline()
{

    // Where the rendering occurs.

}

// --- Helpers -----------------------------------------------------------------

void RendererPipeline::
flush_all_pipelines()
{

    RendererPipeline::flush_point_pipeline();
    RendererPipeline::flush_line_pipeline();

}
