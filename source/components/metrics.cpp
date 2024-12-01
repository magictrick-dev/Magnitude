#include <components/metrics.hpp>

MetricsComponent::
MetricsComponent(i32 index, std::string name) : EditorComponent(index, name)
{

    this->visible = true;

    r32 default_value = 1.0f / 60.0f;
    for (i32 i = 0; i < METRICS_FRAME_TIME_BUFFER_SIZE; ++i)
        this->frame_time_set[i] = default_value;

}

MetricsComponent::
~MetricsComponent()
{

}

void MetricsComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Metrics", &this->visible, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {

        ImGui::MenuItem("Pause Metrics", NULL, &this->pause);

        ImGui::EndMenuBar();
    }

    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::PlotLines("##ftimes", this->frame_time_set, METRICS_FRAME_TIME_BUFFER_SIZE,
            0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 80.0f));
    ImGui::PopItemWidth();
    ImGui::Text("Delta %.2f ms/f", this->frame_time * 1000.0f);
    ImGui::SameLine();
    ImGui::Text("Average %.2f ms/f", this->frame_average * 1000.0f);

    ImGui::End();

}

void MetricsComponent::
update()
{

    return;

}

void MetricsComponent::
set_frame_time(r32 delta)
{

    if (!this->pause)
    {
        if (this->frame_index % 16 == 0) this->frame_time = delta;

        this->frame_time_set[this->frame_index] = delta;
        this->frame_index++;
        
        if (this->frame_index >= METRICS_FRAME_TIME_BUFFER_SIZE)
        {

            this->frame_index = 0;
            r32 sum = 0;
            for (i32 i = 0; i < METRICS_FRAME_TIME_BUFFER_SIZE; ++i)
                sum += this->frame_time_set[i];
            sum /= METRICS_FRAME_TIME_BUFFER_SIZE;
            this->frame_average = sum;

        }
    }

}

