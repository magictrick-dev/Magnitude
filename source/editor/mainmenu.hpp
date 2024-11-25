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
        ImGui::Text("Entry");
        ImGui::SeparatorText("Additional Stuff");
        ImGui::Text("Entry");
        ImGui::Text("Entry");
        ImGui::Text("Entry");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Menu"))
    {
        ImGui::Text("Entry");
        ImGui::SeparatorText("Additional Stuff");
        ImGui::Text("Entry");
        ImGui::Text("Entry");
        ImGui::Text("Entry");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings"))
    {
        ImGui::Text("Entry");
        ImGui::SeparatorText("Additional Stuff");
        ImGui::Text("Entry");
        ImGui::Text("Entry");
        ImGui::Text("Entry");
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
