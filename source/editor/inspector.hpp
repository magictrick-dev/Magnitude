#ifndef MAGNITUDE_EDITOR_INSPECTOR_HPP
#define MAGNITUDE_EDITOR_INSPECTOR_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class InspectorComponent : public EditorComponent
{

    public:
        inline                  InspectorComponent();
        inline virtual         ~InspectorComponent();

        inline virtual void     render()    override;
        inline virtual bool     close()     override;
        inline virtual bool     open()      override;

};

inline InspectorComponent::
InspectorComponent() : EditorComponent(COMPONENT_ID_INSPECTOR, COMPONENT_NAME_INSPECTOR)
{

    this->visible = true;

}

inline InspectorComponent::
~InspectorComponent()
{

}

inline bool InspectorComponent::
close()
{

    this->visible = false;
    return true;

}

inline bool InspectorComponent::
open()
{

    this->visible = true;
    return true;

}

inline void InspectorComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Inspector", &this->visible);

    ImGui::End();

}

#endif
