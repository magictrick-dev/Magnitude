#ifndef MAGNITUDE_EDITOR_SCENEVIEWER_HPP
#define MAGNITUDE_EDITOR_SCENEVIEWER_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <renderer/framebuffer.hpp>
#include <imgui/imgui.h>

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

};

inline SceneViewerComponent::
SceneViewerComponent(i32 id, std::string name) 
    : EditorComponent(id, name), framebuffer(800, 600)
{

    this->visible = true;

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
    ImGui::Begin("Scene", &this->visible);

    // Actually calculate the boundary size so we know how big to render into.
    ImVec2 min_area = ImGui::GetWindowContentRegionMin();
    ImVec2 max_area = ImGui::GetWindowContentRegionMax();

    this->viewer_width = max_area.x - min_area.x;
    this->viewer_height = max_area.y - min_area.y;

    ImVec2 image_size = { (r32)this->viewer_width, (r32)this->viewer_height };
    ImGui::Image((void*)(intptr_t)this->framebuffer.get_texture(),
            image_size, { 0, 1 }, { 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();

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

    this->framebuffer.unbind();

}

#endif
