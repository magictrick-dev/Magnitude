#include <components/component.hpp>
#include <imgui/imgui.h>

EditorComponent::
EditorComponent()
{

}

EditorComponent::
EditorComponent(i32 id)
{

    this->id = id;

}

EditorComponent::
EditorComponent(i32 id, std::string name)
{

    this->id = id;
    this->name = name;
    this->visible = false;

}

EditorComponent::
~EditorComponent()
{

}

bool EditorComponent::
pre_render()
{

    ImGuiWindowFlags flags = 0;
    if (this->menu == true) flags |= ImGuiWindowFlags_MenuBar;

    // Standard setup, overridable.
    if (this->visible == false) return false;
    ImGui::Begin(this->name.c_str(), &this->visible, flags);
    this->focused = ImGui::IsWindowFocused();
    return true;

}

void EditorComponent::
post_render()
{

    ImGui::End();

}

void EditorComponent::
render()
{

    

}
