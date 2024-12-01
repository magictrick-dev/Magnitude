#ifndef MAGNITUDE_COMPONENTS_CONSOLE_HPP
#define MAGNITUDE_COMPONENTS_CONSOLE_HPP
#include <definitions.hpp>
#include <iostream>
#include <utilities/logging.hpp>
#include <components/component.hpp>
#include <imgui/imgui.h>

class ConsoleComponent : public EditorComponent
{

    public:
                         ConsoleComponent(i32 id, std::string name);
        virtual         ~ConsoleComponent();

        virtual void     render() override;
        virtual void     update() override;

    protected:
        bool        auto_scroll;
        bool        scroll_to_bottom;

};

#endif
