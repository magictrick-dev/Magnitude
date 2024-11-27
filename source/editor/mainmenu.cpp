#include <editor/mainmenu.hpp>
#include <editor/editor.hpp>

MainMenuComponent::
MainMenuComponent() : EditorComponent(COMPONENT_ID_MAIN_MENU, COMPONENT_NAME_MAIN_MENU)
{

    this->visible = true;

}

MainMenuComponent::
~MainMenuComponent()
{

}

bool MainMenuComponent::
close()
{

    // Do nothing.
    return false;

}

bool MainMenuComponent::
open()
{

    // Do nothing.
    return false;

}

void MainMenuComponent::
render()
{

    if (this->visible != true) return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::BeginMainMenuBar();
    ImGui::PopStyleVar();
    if (ImGui::BeginMenu("File"))
    {

        if (ImGui::MenuItem("Open", NULL))
        {
            Logger::log_info(LogFlag_None, "Open Triggered.");
        }

        if (ImGui::MenuItem("Save", NULL))
        {
            Logger::log_info(LogFlag_None, "Save Triggered.");
        }

        if (ImGui::MenuItem("Save as", NULL))
        {
            Logger::log_info(LogFlag_None, "Save-as Triggered.");
        }

        if (ImGui::MenuItem("Close", NULL))
        {
            Logger::log_info(LogFlag_None, "Close Triggered.");
        }

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
