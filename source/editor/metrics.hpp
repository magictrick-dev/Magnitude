#ifndef MAGNITUDE_EDITOR_MATRICS_HPP
#define MAGNITUDE_EDITOR_MATRICS_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class MetricsComponent : public EditorComponent
{

    public:
        inline                  MetricsComponent(i32 index, std::string name);
        inline virtual         ~MetricsComponent();

        inline virtual void     render()    override;
        inline virtual bool     close()     override;
        inline virtual bool     open()      override;

};

inline MetricsComponent::
MetricsComponent(i32 index, std::string name) : EditorComponent(index, name)
{

    this->visible = true;

}

inline MetricsComponent::
~MetricsComponent()
{

}

inline bool MetricsComponent::
close()
{

    this->visible = false;
    return true;

}

inline bool MetricsComponent::
open()
{

    this->visible = true;
    return true;

}

inline void MetricsComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Metrics", &this->visible);

    ImGui::End();

}


#endif
