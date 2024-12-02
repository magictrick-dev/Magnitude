#include <components/console.hpp>

ConsoleComponent::
ConsoleComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = true;

}

ConsoleComponent::
~ConsoleComponent()
{

}

void ConsoleComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Console", &this->visible, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("Filters"))
        {
            ImGui::SeparatorText("Levels");
            ImGui::MenuItem("Debug",    NULL);
            ImGui::MenuItem("Info",     NULL);
            ImGui::MenuItem("Warning",  NULL);
            ImGui::MenuItem("Critical", NULL);
            ImGui::MenuItem("Error",    NULL);
            ImGui::SeparatorText("Interface");
            ImGui::MenuItem("Internal", NULL);
            ImGui::MenuItem("Parser",   NULL);
            ImGui::MenuItem("Renderer", NULL);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // --- Scrolling Region ----------------------------------------------------
    const r32 footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + 
        ImGui::GetFrameHeightWithSpacing();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), 
                ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
    {

        std::vector<LogDescriptor> messages = Logger::get_messages();
        for (auto message : messages)
        {
            ImGui::Text(message.message.c_str());
        }

        if (scroll_to_bottom || (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        scroll_to_bottom = false;
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::Separator();

    // --- Command Input Region ------------------------------------------------
    ImGuiInputTextFlags input_text_flags = 
        ImGuiInputTextFlags_EnterReturnsTrue | 
        ImGuiInputTextFlags_EscapeClearsAll;
    static char input_buffer[256];
    static bool first_initialized = false;
    if (first_initialized == false)
    {
        input_buffer[0] = '\0';
        first_initialized = true;
    }

    bool reclaim_focus = false;
    if (ImGui::Button("Run Command"))
    {
        reclaim_focus = true;
        if (strcmp(input_buffer, "") != 0)
        {
            Logger::log_debug(LogFlag_None, input_buffer);
            input_buffer[0] = '\0';
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::InputText("##ConsoleIn", input_buffer, 256, input_text_flags))
    {
        reclaim_focus = true;
        if (strcmp(input_buffer, "") != 0)
        {
            Logger::log_debug(LogFlag_None, input_buffer);
            input_buffer[0] = '\0';
        }
    }
    ImGui::PopItemWidth();

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
    {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();

}

void ConsoleComponent::
update()
{

    return;

}

