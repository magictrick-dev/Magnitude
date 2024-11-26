#ifndef MAGNITUDE_EDITOR_HPP
#define MAGNITUDE_EDITOR_HPP
#include <vector>
#include <string>
#include <unordered_map>
#include <definitions.hpp>
#include <editor/component.hpp>

class Editor
{

    public:
        static Editor&  get();

        void render();   

        virtual    ~Editor();
    protected:
                    Editor();

    protected:
        std::shared_ptr<EditorComponent> main_menu_component;
        std::shared_ptr<EditorComponent> scene_viewer_component;
        std::shared_ptr<EditorComponent> inspector_component;
        std::shared_ptr<EditorComponent> console_component;

        std::vector<std::shared_ptr<EditorComponent>>                       editor_components;
        std::unordered_map<std::string, std::shared_ptr<EditorComponent>>   editor_map;
        i32 editor_step = 1;

};

#endif
