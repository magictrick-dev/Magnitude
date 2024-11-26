#include <editor/component.hpp>

EditorComponent::
EditorComponent()
{

    this->id = -1;
    this->name = "unnamed_component";
    this->visible = false;

}

EditorComponent::
EditorComponent(i32 id)
{

    this->id = id;
    this->name = "unnamed_component";
    this->visible = false;

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
is_visible() const
{

    return this->visible;

}

std::string EditorComponent::
get_name() const
{

    return this->name;

}

i32 EditorComponent::
get_id() const
{

    return this->id;

}
