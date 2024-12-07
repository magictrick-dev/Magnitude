#include <environment.hpp>
#include <components/sceneviewer.hpp>
#include <platform/opengl.hpp>
#include <utilities/logging.hpp>
#include <imgui/imgui.h>

static ccptr vertex_shader = R"(#version 430 core
layout(location = 0) in vec3 a_position;

void main()
{
    gl_Position = vec4(a_position, 1.0f);
}
)";

static ccptr fragment_shader = R"(#version 430 core
out vec4 color;

void main()
{
    color = vec4(0.580f, 0.470f, 0.340f, 1.000f);
}
)";

SceneViewerComponent::
SceneViewerComponent(i32 id, std::string name) 
    : EditorComponent(id, name), framebuffer(800, 600)
{

    this->visible = true;
    this->menu = true;

}

SceneViewerComponent::
~SceneViewerComponent()
{

}

bool SceneViewerComponent::
pre_render()
{

    if (this->visible == false) return false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(this->name.c_str(), &this->visible, ImGuiWindowFlags_MenuBar);
    ImGui::PopStyleVar();
    this->focused = ImGui::IsWindowFocused();
    return true;

}

void SceneViewerComponent::
render()
{

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("Properties"))
        {

            if (ImGui::MenuItem("Static Image", NULL, &this->static_only))
            {
                Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
            }

            if (ImGui::MenuItem("Fix Aspect Ratio", NULL, &this->fixed_aspect_ratio))
            {
                Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
            }

            ImGui::EndMenu();

        }

        ImGui::Separator();

        if (ImGui::MenuItem(ICON_FA_REPEAT, NULL, &this->cycle_frames))
        {
            Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
        }

        if (ImGui::MenuItem(ICON_FA_BACKWARD_STEP, NULL))
        {
            Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
        }

        ccptr free_play_icon = (this->free_camera) ? ICON_FA_PAUSE : ICON_FA_PLAY;
        if (ImGui::MenuItem(free_play_icon, NULL, &this->free_camera))
        {
            Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
        }

        if (ImGui::MenuItem(ICON_FA_FORWARD_STEP, NULL))
        {
            Logger::log_debug(LogFlag_None, "Unimplemented as of yet.");
        }

        ImGui::EndMenuBar();
    }

    // Actually calculate the boundary size so we know how big to render into.
    ImVec2 min_area = ImGui::GetWindowContentRegionMin();
    ImVec2 max_area = ImGui::GetWindowContentRegionMax();

    this->viewer_width = max_area.x - min_area.x;
    this->viewer_height = max_area.y - min_area.y;

    Environment& environment = Environment::get();
    ImVec2 image_size = { (r32)this->viewer_width, (r32)this->viewer_height };
    ImGui::Image((void*)(intptr_t)environment.framebuffer.get_texture(),
            image_size, { 0, 1 }, { 1, 0 });

}

void SceneViewerComponent::
update()
{

    Environment& environment = Environment::get();
    environment.framebuffer.resize(this->viewer_width, this->viewer_height);

}
