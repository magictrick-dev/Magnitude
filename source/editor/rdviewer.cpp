#include <editor/rdviewer.hpp>
#include <utilities/logging.hpp>
#include <platform/filesystem.hpp>

RDViewerComponent::
RDViewerComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = true;
    this->file_path = "";
    this->file_changes = false;

}

RDViewerComponent::
~RDViewerComponent()
{

}

bool RDViewerComponent::
file_unsaved() const
{

    return this->file_changes;

}

bool RDViewerComponent::
save_file()
{

    // No need to save.
    if (this->file_changes == false) return true;

    // Otherwise, we can save.
    Filepath save_location = this->file_path;
    if (!save_location.is_valid_file())
    {

        std::string new_save_location = file_save_as_system_dialogue(".rd", "Render View File .rd\0");

        // TODO(Chris): Confirmation to overwrite here.
        save_location = new_save_location.c_str();

    }

    std::string text = this->file_editor.GetText();
    size_t length = text.length();
    u64 written = file_write_all(save_location.c_str(), text.data(), length - 1);
    if (written == 0)
    {
        Logger::log_error(LogFlag_None, "Unable to save file at %s.", save_location.c_str());
        return false;
    }

    this->file_path = save_location;
    return true;

}

bool RDViewerComponent::
close()
{

    this->visible = false;
    return true;

}

bool RDViewerComponent::
open()
{

    this->visible = true;
    return true;

}

void RDViewerComponent::
render()
{

    if (this->visible == false) return;
    
    ImGuiWindowFlags window_flags = 0;
    if (this->file_changes == true) window_flags |= ImGuiWindowFlags_UnsavedDocument;

    ImGui::Begin("Text Editor", &this->visible, window_flags);

    this->file_editor.Render("Text Editor");

    if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && this->file_changes)
    {
        this->save_file();
        this->file_changes = false;
    }

    ImGui::End();

    if (this->file_editor.IsTextChanged())
    {
        this->file_changes = true;
    }

}

bool RDViewerComponent::
set_file(Filepath path)
{

    if (!path.is_valid_file())
    {
        return false;
    }

    this->file_path = path;

    rhandle resource = ResourceManager::create_file_resource(path);
    if (!ResourceManager::resource_handle_is_valid(resource)) return false;
    if (!ResourceManager::load_resource(resource)) return false;

    this->file_editor.SetText(ResourceManager::get_resource_as_string(resource));

    ResourceManager::release_resource(resource);

    return true;

}

bool RDViewerComponent::
new_file()
{

    this->file_path = "";
    this->file_editor.SetText("");
    return true;

}

