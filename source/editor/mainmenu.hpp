#ifndef MAGNITUDE_EDITOR_MAINMENU_HPP
#define MAGNITUDE_EDITOR_MAINMENU_HPP
#include <definitions.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class MainMenuComponent : public EditorComponent
{

    public:
        inline                  MainMenuComponent();
        inline virtual         ~MainMenuComponent();

        inline virtual void     render()    override;
        inline virtual bool     close()     override;
        inline virtual bool     open()      override;

};

inline MainMenuComponent::
MainMenuComponent() : EditorComponent("main_menu", -1)
{

    this->visible = true;

}

inline MainMenuComponent::
~MainMenuComponent()
{

}

inline bool MainMenuComponent::
close()
{

    // Do nothing.

}

inline bool MainMenuComponent::
open()
{

    // Do nothing.

}

inline void MainMenuComponent::
render()
{

    if (this->visible != true) return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::BeginMainMenuBar();
    ImGui::PopStyleVar();
    if (ImGui::BeginMenu("File"))
    {
        ImGui::Text("The menu is fake, for now.");
        ImGui::Text("Open");
        ImGui::Text("Save");
        ImGui::Text("Save as");
        ImGui::Text("Close File");
        ImGui::Text("Exit");
        ImGui::SeparatorText("Preferences");
        ImGui::Text("Editor Highlighting");
        ImGui::Text("Editor Display Errors Realtime");
        ImGui::Text("Editor Display Help Contexts");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
        ImGui::Text("Editor");
        ImGui::Text("Viewport");
        ImGui::Text("Inspector");
        ImGui::Text("Console");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings"))
    {
        ImGui::Text("Verbose Logs");
        ImGui::Text("Force Hardware Acceleration");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About"))
    {
        ImGui::Text("Magnitude Version A.0.1");
        ImGui::Separator();
        ImGui::Text("Developed by Chris DeJong, magictrick-dev, 2024-Current");
        ImGui::Text("   Magnitude is a scriptable graphics visualizer designed");
        ImGui::Text("   to rapidly iterate, test, and create dynamic scenes in 2D/3D.");
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
    

}

#endif
