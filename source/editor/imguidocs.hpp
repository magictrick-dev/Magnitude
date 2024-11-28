#ifndef MAGNITUDE_EDITOR_IMGUIDOCS_HPP
#define MAGNITUDE_EDITOR_IMGUIDOCS_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class ImguiDocsComponent : public EditorComponent
{

    public:
        inline                  ImguiDocsComponent(i32 id, std::string name);
        inline virtual         ~ImguiDocsComponent();

        inline virtual void     render()    override;

};

inline ImguiDocsComponent::
ImguiDocsComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = false;
    this->shortcut = "F11";

}

inline ImguiDocsComponent::
~ImguiDocsComponent()
{

}

inline void ImguiDocsComponent::
render()
{

    if (this->visible == false) return;
    ImGui::ShowDemoWindow(&this->visible);
    if (ImGui::IsKeyPressed(ImGuiKey_F11))
        this->visible = !this->visible;

}

#endif
