#ifndef MAGNITUDE_EDITOR_CONSOLE_HPP
#define MAGNITUDE_EDITOR_CONSOLE_HPP
#include <definitions.hpp>
#include <iostream>
#include <utilities/logging.hpp>
#include <editor/component.hpp>
#include <imgui/imgui.h>

class ConsoleComponent : public EditorComponent
{

    public:
        inline                  ConsoleComponent();
        inline virtual         ~ConsoleComponent();

        inline virtual void     render()    override;
        inline virtual bool     close()     override;
        inline virtual bool     open()      override;

};

inline ConsoleComponent::
ConsoleComponent() : EditorComponent(-4, "console")
{

    this->visible = true;

}

inline ConsoleComponent::
~ConsoleComponent()
{

}

inline bool ConsoleComponent::
close()
{

    this->visible = false;
    return true;

}

inline bool ConsoleComponent::
open()
{

    this->visible = true;
    return true;

}

inline void ConsoleComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Console", &this->visible);

    const r32 footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + 
        ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), 
                ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
    {

        std::vector<LogDescriptor> messages = Logger::get_messages();
        for (auto message : messages)
        {
            ImGui::Text(message.message.c_str());
        }

    }

    ImGui::EndChild();
    ImGui::Separator();

    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | 
        ImGuiInputTextFlags_EscapeClearsAll;

    static char input_buffer[256];
    static bool first_initialized = false;
    if (first_initialized == false)
    {
        input_buffer[0] = '\0';
        first_initialized = true;
    }

    bool reclaim_focus = false;
    if (ImGui::InputText("Input", input_buffer, 256, input_text_flags))
    {
        reclaim_focus = true;
        if (strcmp(input_buffer, "") != 0)
        {
            Logger::log_debug(LogFlag_None, input_buffer);
            input_buffer[0] = '\0';
        }
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
    {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();

}


#endif
