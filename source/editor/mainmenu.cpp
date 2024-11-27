#include <editor/mainmenu.hpp>
#include <editor/editor.hpp>
#include <platform/filesystem.hpp>
#include <environment.hpp>

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
            Environment::rdview_file_open();
        }

        if (ImGui::MenuItem("Save", NULL))
        {
            Logger::log_info(LogFlag_None, "Saving...");
        }

        if (ImGui::MenuItem("Save as", NULL))
        {
            std::string save_path = file_save_as_system_dialogue(".rd", "Render View File .rd\0");
            Logger::log_info(LogFlag_None, "Saving as... %s.", save_path.c_str());
        }

        if (ImGui::MenuItem("Close", NULL))
        {
            Logger::log_info(LogFlag_None, "Closing...");
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
