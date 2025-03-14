#ifndef MAGNITUDE_COMPONENTS_IMGUIDOCS_HPP
#define MAGNITUDE_COMPONENTS_IMGUIDOCS_HPP
#include <definitions.hpp>
#include <platform/input.hpp>
#include <components/component.hpp>
#include <utilities/logging.hpp>
#include <imgui/imgui.h>

class ImguiDocsComponent : public EditorComponent
{

    public:
        inline                  ImguiDocsComponent(i32 id, std::string name);
        inline virtual         ~ImguiDocsComponent();

        inline virtual void     render()    override;
        inline virtual void     update()    override;
        inline virtual bool     pre_render() override;
        inline virtual void     post_render() override;

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

inline bool ImguiDocsComponent::
pre_render()
{
    return true;
}

inline void ImguiDocsComponent::
post_render()
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

    if (input_key_is_pressed(MagKeyF9))
        this->visible = !this->visible;

}

#endif
