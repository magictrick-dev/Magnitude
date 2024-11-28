#include <platform/filesystem.hpp>
#include <editor/mainmenu.hpp>
#include <editor/editor.hpp>
#include <editor/rdviewer.hpp>


MainMenuComponent::
MainMenuComponent(i32 id, std::string name) : EditorComponent(id, name)
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

    auto rdviewer = Editor::get_component_by_name<RDViewerComponent>("rdviewer");
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::BeginMainMenuBar();
    ImGui::PopStyleVar();

    std::string stem = rdviewer->get_file_path().get_file_stem();
    if (stem.empty()) stem = "Unopened Project";
    ImGui::Text(stem.c_str());
    ImGui::Separator();
    ImGui::Separator();

    if (ImGui::BeginMenu("File"))
    {

        if (ImGui::MenuItem("New", NULL))
        {
            rdviewer->new_file();
        }

        if (ImGui::MenuItem("Open", NULL))
        {

            // Request the file.
            std::string result = file_open_system_dialogue();
            Filepath path = result.c_str();

            // Attempt to set the file.
            rdviewer->set_file(path);

        }

        bool disable_save = false;
        if (!rdviewer->file_unsaved()) disable_save = true;

        if (disable_save) ImGui::BeginDisabled();
        if (ImGui::MenuItem("Save", NULL))
        {
            rdviewer->save_file();
        }
        if (disable_save) ImGui::EndDisabled();

        if (ImGui::MenuItem("Save as", NULL))
        {
            rdviewer->save_as_file();
        }

        if (ImGui::MenuItem("Close", NULL))
        {
            rdviewer->close_file();
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
