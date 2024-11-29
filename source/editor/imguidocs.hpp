#ifndef MAGNITUDE_EDITOR_IMGUIDOCS_HPP
#define MAGNITUDE_EDITOR_IMGUIDOCS_HPP
#include <definitions.hpp>
#include <platform/input.hpp>
#include <editor/component.hpp>
#include <utilities/logging.hpp>
#include <imgui/imgui.h>

class ImguiDocsComponent : public EditorComponent
{

    public:
        inline                  ImguiDocsComponent(i32 id, std::string name);
        inline virtual         ~ImguiDocsComponent();

        inline virtual void     render()    override;
        inline virtual void     update()    override;

};

inline ImguiDocsComponent::
ImguiDocsComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = false;
    this->shortcut = "F9";

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

}

inline void ImguiDocsComponent::
update()
{

    if (input_key_is_pressed(MagKeyF8))
        this->visible = !this->visible;

}

#endif
