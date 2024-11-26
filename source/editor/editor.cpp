#include <editor/editor.hpp>
#include <editor/mainmenu.hpp>
#include <editor/sceneviewer.hpp>
#include <editor/inspector.hpp>

Editor::
Editor()
{

    this->main_menu_component       = std::make_shared<MainMenuComponent>();
    this->scene_viewer_component    = std::make_shared<SceneViewerComponent>();
    this->inspector_component       = std::make_shared<InspectorComponent>();

}

Editor::
~Editor()
{

}

Editor& Editor::
get()
{

    static std::shared_ptr<Editor> instance = nullptr;
    if (instance == nullptr)
    {

        Editor *editor = new Editor();
        instance = std::shared_ptr<Editor>(editor);

    }

    return *instance;

}

void Editor::
render()
{

    // Core components.
    this->main_menu_component->render();
    this->scene_viewer_component->render();
    this->inspector_component->render();

    // Snap-ins.
    for (auto component : this->editor_components)
        component->render();

}

