#include <platform/filesystem.hpp>
#include <editor.hpp>
#include <components/mainmenu.hpp>
#include <components/texteditor.hpp>


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
is_toggleable()
{

    return false;

}

bool MainMenuComponent::
is_menuable()
{
    return false;
}

bool MainMenuComponent::
toggle()
{
    return false;
}

bool MainMenuComponent::
open()
{
    return false;
}

bool MainMenuComponent::
close()
{
    return false;
}

void MainMenuComponent::
render()
{

    if (this->visible != true) return;

    auto rdviewer = Editor::get_component_by_name<TextEditorComponent>("Editor");
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::BeginMainMenuBar();
    ImGui::PopStyleVar();

    std::string stem = rdviewer->get_file_path().get_file_stem();
    if (stem.empty()) stem = "Unopened Project";
    ImGui::TextColored(ImVec4(0.510f, 0.510f, 0.510f, 1.000f), stem.c_str());
    ImGui::Separator();

    if (ImGui::BeginMenu("File"))
    {

        if (ImGui::MenuItem("New", "Ctrl-N"))
        {
            rdviewer->new_file();
        }

        if (ImGui::MenuItem("Open", "Ctrl-O"))
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
        if (ImGui::MenuItem("Save", "Ctrl-S"))
        {
            rdviewer->save_file();
        }
        if (disable_save) ImGui::EndDisabled();

        if (ImGui::MenuItem("Save as", "Ctrl-Shift-S"))
        {
            rdviewer->save_as_file();
        }

        if (ImGui::MenuItem("Close Document", NULL))
        {
            rdviewer->close_file();
        }

        ImGui::EndMenu();

    }

    if (ImGui::BeginMenu("View"))
    {

        auto component_list = Editor::get_component_list();
        for (auto component : component_list)
        {

            if (!component->is_menuable()) continue;
            if (!component->is_toggleable()) ImGui::BeginDisabled();
            bool status = component->is_visible();
            if (ImGui::MenuItem(component->get_name().c_str(),
                                component->get_menu_shortcut().c_str(), &status))
            {
                component->toggle();
            }
            if (!component->is_toggleable()) ImGui::BeginDisabled();

        }

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

void MainMenuComponent::
update()
{

    return;

}

