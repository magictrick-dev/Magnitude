#ifndef MAGNITUDE_COMPONENTS_MAINMENU_HPP
#define MAGNITUDE_COMPONENTS_MAINMENU_HPP
#include <definitions.hpp>
#include <utilities/logging.hpp>
#include <components/component.hpp>
#include <imgui/imgui.h>

class MainMenuComponent : public EditorComponent
{

    public:
                         MainMenuComponent(i32 id, std::string name);
        virtual         ~MainMenuComponent();

        virtual void     render() override;
        virtual void     update() override;

        virtual bool     open() override;
        virtual bool     close() override;
        virtual bool     toggle() override;
        virtual bool     is_toggleable() override;
        virtual bool     is_menuable() override;

};

#endif
