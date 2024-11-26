#ifndef MAGNITUDE_EDITOR_MAINMENU_HPP
#define MAGNITUDE_EDITOR_MAINMENU_HPP
#include <definitions.hpp>
#include <imgui/imgui.h>

class EditorComponent
{
    public:
        inline virtual ~EditorComponent() {};
        virtual void    render() = 0;
        inline bool     is_visible() const { return this->visible; }

    protected:
        bool        visible;
};

class MainMenuComponent : public EditorComponent
{

    public:
        inline                  MainMenuComponent();
        inline virtual         ~MainMenuComponent();
        inline virtual void     render() override;

};

inline MainMenuComponent::
MainMenuComponent()
{

    this->visible = true;

}

inline MainMenuComponent::
~MainMenuComponent()
{

    this->visible = true;

}

inline void MainMenuComponent::
render()
{

    if (this->visible != true) return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::BeginMainMenuBar();
    ImGui::PopStyleVar();
    ImGui::TextColored(ImVec4(0.580f, 0.470f, 0.340f, 1.000f), " Magnitude ");
    ImGui::Separator();
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
