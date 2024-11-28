#include <editor/rdviewer.hpp>
#include <utilities/logging.hpp>
#include <platform/input.hpp>
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

Filepath RDViewerComponent::
get_file_path() const
{

    return this->file_path;

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
        save_location = new_save_location.c_str();
        if (save_location.is_valid_file())
        {

            if (!file_confirm_message("Save", 
                "Are you sure you want to overwrite an existing file?"))
                    return false;

        }

    }

    std::string text = this->file_editor.GetText();
    size_t length = text.length();
    u64 written = file_write_all(save_location.c_str(), text.data(), length - 1);
    if (written == 0)
    {
        Logger::log_error(LogFlag_None, "Unable to save file at %s.", save_location.c_str());
        return false;
    }

    Logger::log_info(LogFlag_None, "Successfully saved changes to %s.", save_location.c_str());
    this->file_path = save_location;
    this->file_changes = false;
    return true;

}

bool RDViewerComponent::
save_as_file()
{

    // Otherwise, we can save.
    std::string new_save_location = file_save_as_system_dialogue(".rd", "Render View File .rd\0");
    Filepath save_location = new_save_location.c_str();

    if (save_location.is_valid_file())
    {

        bool result = file_confirm_message("Save as", "Are you sure you want to overwrite an existing file?");
        if (result == false)
            return false;

    }

    std::string text = this->file_editor.GetText();
    size_t length = text.length();
    u64 written = file_write_all(save_location.c_str(), text.data(), length - 1);
    if (written == 0)
    {
        Logger::log_error(LogFlag_None, "Unable to save file at %s.", save_location.c_str());
        return false;
    }

    Logger::log_info(LogFlag_None, "Successfully saved changes to %s.", save_location.c_str());
    this->file_path = save_location;
    this->file_changes = false;
    return true;

}

bool RDViewerComponent::
close_file()
{

    bool actually_close = true;
    if (this->file_changes)
    {

        bool choice = file_confirm_message("Close", 
                "Are you sure you want to close without saving changes?");
        actually_close = choice;

    }

    if (actually_close)
    {
        this->file_editor.SetText("");
        this->file_path = "";
        Logger::log_info(LogFlag_None, "The project source has successfully closed.");
        return true;
    }

    return false;

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
    Logger::log_info(LogFlag_None, "The project source has been set to %s.", path.c_str());

    ResourceManager::release_resource(resource);
    this->file_changes = false;

    return true;

}

bool RDViewerComponent::
new_file()
{

    if (this->file_changes)
    {

        bool choice = file_confirm_message("New", 
                "Are you sure you want to create a new view without saving changes?");
        if (choice == false) return false;

    }

    this->file_path = "";
    this->file_editor.SetText("");
    this->file_changes = false;
    Logger::log_info(LogFlag_None, "A new project has been created.");
    return true;

}

void RDViewerComponent::
render()
{

    if (this->visible == false) return;
    
    ImGuiWindowFlags window_flags = 0;
    if (this->file_changes == true) window_flags |= ImGuiWindowFlags_UnsavedDocument;

    ImGui::Begin("Text Editor", &this->visible, window_flags);

    ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    this->file_editor.Render("Text Editor");
    ImGui::PopStyleColor();

    ImGui::End();

    // Requires after render since property is an ImGui component with internal
    // state outside of update.
    if (this->file_editor.IsTextChanged())
    {
        this->file_changes = true;
    }

}

void RDViewerComponent::
update()
{


    if (input_key_is_released(MagKeyS) && 
        input_key_is_down(MagKeyControl) && 
        this->file_changes)
    {
        this->save_file();
    }

    if (input_key_is_released(MagKeyS) && 
        input_key_is_down(MagKeyControl) && 
        input_key_is_down(MagKeyShift))
    {
        this->save_as_file();
    }

    if (input_key_is_released(MagKeyN) && 
        input_key_is_down(MagKeyControl))
    {
        this->new_file();
    }

    if (input_key_is_released(MagKeyO) && 
        input_key_is_down(MagKeyControl))
    {
        // Request the file.
        std::string result = file_open_system_dialogue();
        Filepath path = result.c_str();

        // Attempt to set the file.
        this->set_file(path);
    }

}
