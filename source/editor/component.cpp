#include <editor/component.hpp>

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
