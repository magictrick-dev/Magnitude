#ifndef MAGNITUDE_EDITOR_MAINMENU_HPP
#define MAGNITUDE_EDITOR_MAINMENU_HPP
#include <definitions.hpp>
#include <utilities/logging.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class MainMenuComponent : public EditorComponent
{

    public:
                         MainMenuComponent();
        virtual         ~MainMenuComponent();

        virtual void     render()    override;
        virtual bool     close()     override;
        virtual bool     open()      override;

};

#endif
