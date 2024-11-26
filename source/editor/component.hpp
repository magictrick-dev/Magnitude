#ifndef MAGNITUDE_EDITOR_COMPONENT_HPP
#define MAGNITUDE_EDITOR_COMPONENT_HPP
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
                            EditorComponent(std::string name, i32 id);
        virtual            ~EditorComponent();

        bool                is_visible() const;
        i32                 get_id() const;
        std::string         get_name() const;

        virtual void        render() = 0;
        virtual bool        close() = 0;
        virtual bool        open() = 0;
        

    protected:
        bool        visible;
        std::string name;
        i32         id;

};

#endif
