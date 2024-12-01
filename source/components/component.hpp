#ifndef MAGNITUDE_EDITOR_COMPONENT_HPP
#define MAGNITUDE_EDITOR_COMPONENT_HPP
#include <string>
#include <definitions.hpp>

// --- Editor Components -------------------------------------------------------
//
// Since the editor itself is a functionally important piece of the application,
// the component system allows us to create components that allows us to generate
// functional UI components with Dear ImGUI in a way that Omar expressly told us
// was not the idea behind its use.
//
// When components are created, they generally own the responsibility of how they
// interact with UI. Some components are critical components, such as the main menu
// bar. This particular component has an ID of -1. Since IDs are meant to be unique,
// negative numbers are technically reserved for components that the editor creates
// for you. Positive numbers are snap-in components and extensions.
//
// 
//

class EditorComponent
{
    public:
                            EditorComponent();
                            EditorComponent(i32 id);
                            EditorComponent(i32 id, std::string);
        virtual            ~EditorComponent();

        inline bool                is_visible() const { return this->visible; }
        inline i32                 get_id() const { return this->id; }
        inline std::string         get_name() const { return this->name; }
        inline std::string         get_menu_shortcut() const { return this->shortcut; }

        inline virtual void render() = 0;
        inline virtual void update() = 0;

        inline virtual bool close()             { return this->visible = false; };
        inline virtual bool open()              { return this->visible = true; };
        inline virtual bool toggle()            { return this->visible = !this->visible; }
        inline virtual bool is_visible()        { return this->visible; }
        inline virtual bool is_toggleable()     { return true; }
        inline virtual bool is_menuable()       { return true; }

    protected:
        std::string name        = "Unset Component Title";
        std::string shortcut    = "";
        i32         id          = -1;
        bool        visible     = true;

};

#endif
