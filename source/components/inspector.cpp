#include <editor.hpp>
#include <components/texteditor.hpp>
#include <components/inspector.hpp>
#include <rdparser/traversals/framesearch.hpp>

InspectorComponent::
InspectorComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = true;

}

InspectorComponent::
~InspectorComponent()
{

}

void InspectorComponent::
render_camera_properties(RDSyntaxNodeFrame *current_frame)
{
    
    ImGui::SeparatorText("Camera Properties");
    ImGui::PushItemWidth(ImGui::GetFontSize() * -8);

    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "Camera properties designate the way the frame "
                "is represented. These are set in the rd-view file "
                "and are updated to the current frame's camera settings. "
                "The default values are assumed when no camera settings "
                "are listed by the user. They are automatically set as the "
                "scene viewer's camera settings unless an override is triggered."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    
    ImGui::InputFloat3("Camera Look-at", (r32*)&RendererPipeline::camera_at);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "Camera look-at points the camera at the point provided."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }


    ImGui::InputFloat3("Camera Position", (r32*)&RendererPipeline::camera_eye);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "Camera position dictates where in 3D space the camera is located."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::InputFloat3("Camera Up", (r32*)&RendererPipeline::camera_up);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "The camera up designates what the viewport considers 'up'. "
                "Generally, you wouldn't want to change this value unless you "
                "wanted to change what axis points up relative to the camera."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::SliderFloat("Camera FOV", &RendererPipeline::camera_fov, 30.0f, 150.0f);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "Ctrl-click will allow you to enter a value manually.\n\n"
                "The camera FOV adjusts the FOV of the camera. Larger angles widen "
                "lensing of the camera, shorter does the opposite."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::InputFloat("Clipping Near", &RendererPipeline::clipping_near);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "The near-plane clipping area clips geometry less than this value."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::InputFloat("Clipping Far", &RendererPipeline::clipping_far);
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(
                "The far-plane clipping area clips geometry greater than this value. "
                "A larger value is to be expected here."
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::PopItemWidth();

}

void InspectorComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Inspector", &this->visible);

    auto text_editor = Editor::get_component_by_name<TextEditorComponent>("Editor");

    static int node_open = 0;
    int current_index = 0;
    if (text_editor->is_parse_valid())
    {
        RDSyntaxParser *parser = text_editor->get_parser();
        RDSyntaxFrameSearch frame_visitor;
        parser->visit_root(&frame_visitor);

        for (auto frame : frame_visitor.frames)
        {

            ImGui::PushID(current_index);

            if (node_open != current_index)
                ImGui::SetNextItemOpen(false, ImGuiCond_Always);
            if (node_open == current_index) ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNodeEx((void*)(intptr_t)current_index,
                        ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth, 
                        "Frame #%d", frame->index))
            {
                ImGui::Unindent();
                node_open = current_index;
                this->render_camera_properties(frame);
                ImGui::Indent();
                ImGui::TreePop();
            }
            ImGui::PopID();
            current_index++;

        }

    }
    

    ImGui::End();

}

void InspectorComponent::
update()
{

}

