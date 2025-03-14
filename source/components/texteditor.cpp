#include <environment.hpp>
#include <components/texteditor.hpp>

#include <utilities/logging.hpp>
#include <rdparser/rdparser.hpp>

#include <platform/input.hpp>
#include <platform/filesystem.hpp>

static ccptr new_default = R"(# Basic, 24fps, 1280x720 display.
Display "Untitled Project" 24 1280 720

FrameBegin 1

    # Camera settings for the frame.
    CameraEye 320 240 -240
    CameraAt  320 240 0
    CameraUp  0.0 -1.0 0.0
    CameraFOV 90

    # Begin the world.
    WorldBegin
        
        # Display a single point at (300,300,0) with radius 1.
        Point 300 300 0 1

    WorldEnd
FrameEnd
)";

TextEditorComponent::
TextEditorComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = true;
    this->file_path = "";
    this->file_changes = true;
    this->file_editor.SetText(new_default);

}

TextEditorComponent::
~TextEditorComponent()
{

}

Filepath TextEditorComponent::
get_file_path() const
{

    return this->file_path;

}


bool TextEditorComponent::
file_unsaved() const
{

    return this->file_changes;

}

bool TextEditorComponent::
save_file()
{

    // No need to save.
    if (this->file_changes == false) return true;

    // Otherwise, we can save.
    Filepath save_location = this->file_path;
    if (!save_location.is_valid_file())
    {

        std::string new_save_location = file_save_as_system_dialogue("rd", "Render View File .rd\0");
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

bool TextEditorComponent::
save_as_file()
{

    // Otherwise, we can save.
    std::string new_save_location = file_save_as_system_dialogue(".rd", "Render View File .rd\0");
    if (new_save_location.empty()) return false;
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

bool TextEditorComponent::
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
        this->file_editor.SetText(new_default);
        this->file_path = "";
        this->file_changes = true;
        this->parser.reset(this->file_editor.GetText());
        this->validate_and_parse();
        Logger::log_info(LogFlag_None, "The project source has successfully closed.");
        return true;
    }

    return false;

}

bool TextEditorComponent::
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
    this->parser.reset(this->file_editor.GetText());
    this->validate_and_parse();
    this->file_changes = false;

    return true;

}

bool TextEditorComponent::
new_file()
{

    if (this->file_changes)
    {

        bool choice = file_confirm_message("New", 
                "Are you sure you want to create a new view without saving changes?");
        if (choice == false) return false;

    }

    this->file_path = "";
    this->file_editor.SetText(new_default);
    this->file_changes = true;
    this->parser.reset(this->file_editor.GetText());
    this->validate_and_parse();
    Logger::log_info(LogFlag_None, "A new project has been created.");
    return true;

}

bool TextEditorComponent::
pre_render()
{

    if (this->visible == false) return false;
    ImGuiWindowFlags window_flags = 0;
    if (this->file_changes == true) window_flags |= ImGuiWindowFlags_UnsavedDocument;
    ImGui::Begin(this->name.c_str(), &this->visible, window_flags | ImGuiWindowFlags_MenuBar);
    this->focused = ImGui::IsWindowFocused();
    return true;

}

void TextEditorComponent::
post_render()
{

    ImGui::End();

}

void TextEditorComponent::
render()
{

    if (ImGui::BeginMenuBar())
    {
        ImGui::EndMenuBar();
    }


    ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    this->file_editor.Render("Text Editor");
    ImGui::PopStyleColor();

    // Requires after render since property is an ImGui component with internal
    // state outside of update.
    if (this->file_editor.IsTextChanged())
    {
        this->file_changes = true;
        this->parser.reset(this->file_editor.GetText());
        this->validate_and_parse();
    }

}

void TextEditorComponent::
update()
{


    if (input_key_is_pressed(MagKeyS) && 
        input_key_is_down(MagKeyControl) && 
        this->file_changes)
    {
        this->save_file();
    }

    if (input_key_is_pressed(MagKeyS) && 
        input_key_is_down(MagKeyControl) && 
        input_key_is_down(MagKeyShift))
    {
        this->save_as_file();
    }

    if (input_key_is_pressed(MagKeyN) && 
        input_key_is_down(MagKeyControl))
    {
        this->new_file();
    }

    if (input_key_is_pressed(MagKeyO) && 
        input_key_is_down(MagKeyControl))
    {
        // Request the file.
        std::string result = file_open_system_dialogue();
        if (!result.empty())
        {
            Filepath path = result.c_str();

            // Attempt to set the file.
            this->set_file(path);
        }

    }

}

bool TextEditorComponent::
is_parse_valid() const
{
    return this->parser.is_valid();
}

RDSyntaxParser* TextEditorComponent::
get_parser()
{
    return &this->parser;
}

bool TextEditorComponent::
validate_and_parse()
{

    if (this->parser.construct_ast())
    {
        Environment& environment = Environment::get();
        environment.parser = this->parser;
        return true;
    }

    return false;

}

