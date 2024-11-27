#ifndef MAGNITUDE_EDITOR_HPP
#define MAGNITUDE_EDITOR_HPP
#include <vector>
#include <string>
#include <unordered_map>
#include <definitions.hpp>
#include <utilities/logging.hpp>
#include <editor/component.hpp>

#define COMPONENT_ID_MAIN_MENU      -1
#define COMPONENT_ID_SCENE_VIEWER   -2
#define COMPONENT_ID_INSPECTOR      -3
#define COMPONENT_ID_CONSOLE        -4
#define COMPONENT_NAME_MAIN_MENU    "main_menu"
#define COMPONENT_NAME_SCENE_VIEWER "scene_viewer"
#define COMPONENT_NAME_INSPECTOR    "inspector"
#define COMPONENT_NAME_CONSOLE      "console"

class Editor
{

    public:
        static Editor&  get();

        void render();   

        template <class T> inline std::shared_ptr<T> get_component_by_name(std::string name) const;
        template <class T> inline std::shared_ptr<T> get_component_by_id(i32 idx) const;
        template <class T, class... Args> inline bool add_component(std::string name, Args... args);

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

template <class T> std::shared_ptr<T> Editor::
get_component_by_name(std::string name) const
{

    auto result = this->editor_map.find(name);
    if (result == this->editor_map.end())
    {
        return nullptr;
    }

    std::shared_ptr<T> comp = dynamic_pointer_cast<T>(result->second);
    return comp;

}

template <class T> std::shared_ptr<T> Editor::
get_component_by_id(i32 idx) const
{

    for (auto comp : this->editor_components)
    {

        if (comp->get_id() == idx)
        {
            std::shared_ptr<T> comp = dynamic_pointer_cast<T>(comp);
            return comp;
        }
        
    }

    return nullptr;

}

template <class T, class... Args> bool Editor::
add_component(std::string name, Args... args)
{

    if (this->get_component_by_name<T>(name) == nullptr)
    {

        std::shared_ptr<T> new_comp = std::make_shared<T>(this->editor_step++, name, &args...); 
        std::shared_ptr<EditorComponent> pushable = dynamic_pointer_cast<EditorComponent>(new_comp);
        this->editor_components.push_back(pushable);
        this->editor_map[name] = pushable;

        return true;

    }

    Logger::log_warning(LogFlag_None, "Attempted to add an existing component with the same name '%s'.", name.c_str()); 
    return false;

}

#endif
