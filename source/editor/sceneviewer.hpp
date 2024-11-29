#ifndef MAGNITUDE_EDITOR_SCENEVIEWER_HPP
#define MAGNITUDE_EDITOR_SCENEVIEWER_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <renderer/framebuffer.hpp>
#include <imgui/imgui.h>

ccptr vertex_shader = R"(#version 430 core
layout(location = 0) in vec3 a_position;

void main()
{
    gl_Position = vec4(a_position, 1.0f);
}
)";

ccptr fragment_shader = R"(#version 430 core
out vec4 color;

void main()
{
    color = vec4(0.580f, 0.470f, 0.340f, 1.000f);
}
)";

class SceneViewerComponent : public EditorComponent
{

    public:
        inline                  SceneViewerComponent(i32 id, std::string name);
        inline virtual         ~SceneViewerComponent();

        inline virtual void     render()    override;
        inline virtual void     update()    override;

    protected:
        Framebuffer framebuffer;
        i32 viewer_width = 100;
        i32 viewer_height = 100;

        bool static_only        = false;
        bool fixed_aspect_ratio = true;

    protected:
        u32 vtx_buffer;
        u32 prog;

};

inline SceneViewerComponent::
SceneViewerComponent(i32 id, std::string name) 
    : EditorComponent(id, name), framebuffer(800, 600)
{

    this->visible = true;

    static const GLfloat g_vertex_buffer_data[] = {
       -0.5f, -0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        0.0f,  0.5f,  0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    this->vtx_buffer = vertexbuffer;

    u32 vtx_shader = OpenGLRenderContext::shader_create(GL_VERTEX_SHADER);
    u32 frg_shader = OpenGLRenderContext::shader_create(GL_FRAGMENT_SHADER);
    OpenGLRenderContext::shader_compile(vtx_shader, vertex_shader);
    OpenGLRenderContext::shader_compile(frg_shader, fragment_shader);
    this->prog = OpenGLRenderContext::program_create();

    OpenGLRenderContext::program_attach(this->prog, vtx_shader);
    OpenGLRenderContext::program_attach(this->prog, frg_shader);
    OpenGLRenderContext::program_link(this->prog);
    OpenGLRenderContext::shader_release(vtx_shader);
    OpenGLRenderContext::shader_release(frg_shader);


}

inline SceneViewerComponent::
~SceneViewerComponent()
{

}

inline void SceneViewerComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene", &this->visible, ImGuiWindowFlags_MenuBar);
    ImGui::PopStyleVar();

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("Properties"))
        {

            if (ImGui::MenuItem("Static Image", NULL, &this->static_only))
            {
                Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
            }

            if (ImGui::MenuItem("Fix Aspect Ratio", NULL, &this->fixed_aspect_ratio))
            {
                Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
            }

            ImGui::EndMenu();

        }

        ImGui::Separator();


        ImGui::EndMenuBar();
    }

    // Actually calculate the boundary size so we know how big to render into.
    ImVec2 min_area = ImGui::GetWindowContentRegionMin();
    ImVec2 max_area = ImGui::GetWindowContentRegionMax();

    this->viewer_width = max_area.x - min_area.x;
    this->viewer_height = max_area.y - min_area.y;

    ImVec2 image_size = { (r32)this->viewer_width, (r32)this->viewer_height };
    ImGui::Image((void*)(intptr_t)this->framebuffer.get_texture(),
            image_size, { 0, 1 }, { 1, 0 });

    ImGui::End();

}

inline void SceneViewerComponent::
update()
{

    this->framebuffer.bind();
    if (this->framebuffer.get_width() != this->viewer_width ||
        this->framebuffer.get_height() != this->viewer_height)
    {
        this->framebuffer.resize(this->viewer_width, this->viewer_height);
    }

    glViewport(0, 0, this->framebuffer.get_width(), this->framebuffer.get_height());
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(this->prog);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vtx_buffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    glUseProgram(NULL);

    this->framebuffer.unbind();

}

#endif
