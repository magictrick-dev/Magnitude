#ifndef MAGNITUDE_EDITOR_INSPECTOR_HPP
#define MAGNITUDE_EDITOR_INSPECTOR_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

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

    ImGui::End();

}

inline void InspectorComponent::
update()
{

}

#endif
