#ifndef MAGNITUDE_EDITOR_HPP
#define MAGNITUDE_EDITOR_HPP
#include <vector>
#include <string>
#include <unordered_map>
#include <definitions.hpp>
#include <utilities/logging.hpp>
#include <editor/component.hpp>

class Editor
{

    public:
        static inline void render();   
        static inline void update();   

        static inline std::vector<shared_ptr<EditorComponent>>  get_component_list();
        template <class T> static inline shared_ptr<T>          get_component_by_name(std::string name);
        template <class T> static inline shared_ptr<T>          get_component_by_id(i32 idx);
        template <class T, class... Args> static inline bool    add_component(std::string name, Args... args);

        virtual inline             ~Editor();
    protected:
        static inline Editor&       get();
        inline                      Editor();

    protected:
        std::vector<shared_ptr<EditorComponent>> editor_components;
        std::unordered_map<std::string, shared_ptr<EditorComponent>> editor_map;
        i32 editor_step = 1;

};

std::vector<shared_ptr<EditorComponent>> Editor::
get_component_list()
{

    Editor& editor = Editor::get();
    return editor.editor_components;

}

template <class T> shared_ptr<T> Editor::
get_component_by_name(std::string name)
{

    Editor& editor = Editor::get();

    auto result = editor.editor_map.find(name);
    MAG_ASSERT(result != editor.editor_map.end());
    if (result == editor.editor_map.end())
    {
        return nullptr;
    }

    shared_ptr<T> comp = dynamic_pointer_cast<T>(result->second);
    MAG_ENSURE_PTR(comp);
    return comp;

}

template <class T> shared_ptr<T> Editor::
get_component_by_id(i32 idx)
{

    Editor& editor = Editor::get();
    for (auto comp : editor.editor_components)
    {

        if (comp->get_id() == idx)
        {
            shared_ptr<T> comp = dynamic_pointer_cast<T>(comp);
            MAG_ENSURE_PTR(comp);
            return comp;
        }
        
    }

    return nullptr;

}

template <class T, class... Args> bool Editor::
add_component(std::string name, Args... args)
{

    Editor& editor = Editor::get();

    auto search_results = editor.editor_map.find(name);
    if (search_results == editor.editor_map.end())
    {

        shared_ptr<T> new_comp = std::make_shared<T>(editor.editor_step++, name.c_str(), &args...); 
        shared_ptr<EditorComponent> pushable = dynamic_pointer_cast<EditorComponent>(new_comp);
        editor.editor_components.push_back(pushable);
        editor.editor_map[name] = pushable;

        return true;

    }

    Logger::log_warning(LogFlag_None, "Attempted to add an existing component with the same name '%s'.", name.c_str()); 
    return false;

}

Editor::
Editor()
{

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
    
    Editor& editor = Editor::get();

    // Snap-ins.
    for (auto component : editor.editor_components)
        component->render();

}

void Editor::
update()
{
    
    Editor& editor = Editor::get();

    // Snap-ins.
    for (auto component : editor.editor_components)
        component->update();

}

#endif
