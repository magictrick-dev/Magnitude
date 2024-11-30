#ifndef MAGNITUDE_EDITOR_INSPECTOR_HPP
#define MAGNITUDE_EDITOR_INSPECTOR_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>
#include <renderer/pipeline.hpp>

class InspectorComponent : public EditorComponent
{

    public:
        inline                  InspectorComponent(i32 id, std::string name);
        inline virtual         ~InspectorComponent();

        inline virtual void     render()    override;
        inline virtual void     update()    override;

};

inline InspectorComponent::
InspectorComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = true;

}

inline InspectorComponent::
~InspectorComponent()
{

}

inline void InspectorComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Inspector", &this->visible);
    ImGui::PushItemWidth(ImGui::GetFontSize() * -8);
    
    ImGui::SeparatorText("Camera Properties");
    ImGui::InputFloat3("Camera Look-at", (r32*)&RendererPipeline::camera_at);
    ImGui::InputFloat3("Camera Position", (r32*)&RendererPipeline::camera_eye);
    ImGui::InputFloat3("Camera Up", (r32*)&RendererPipeline::camera_up);
    ImGui::SliderFloat("Camera FOV", &RendererPipeline::camera_fov, 30.0f, 150.0f);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("Ctrl-click to enter value manually.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::InputFloat("Clipping Near", &RendererPipeline::clipping_near);
    ImGui::InputFloat("Clipping Far", &RendererPipeline::clipping_far);

    ImGui::PopItemWidth();
    ImGui::End();

}

inline void InspectorComponent::
update()
{

}

#endif
