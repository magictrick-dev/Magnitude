#ifndef MAGNITUDE_EDITOR_SCENEVIEWER_HPP
#define MAGNITUDE_EDITOR_SCENEVIEWER_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class SceneViewerComponent : public EditorComponent
{

    public:
        inline                  SceneViewerComponent(i32 id, std::string name);
        inline virtual         ~SceneViewerComponent();

        inline virtual void     render()    override;

};

inline SceneViewerComponent::
SceneViewerComponent(i32 id, std::string name) : EditorComponent(id, name)
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
    
    ImGui::Begin("Scene", &this->visible);

    ImGui::End();

}

#endif
