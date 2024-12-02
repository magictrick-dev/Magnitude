#ifndef MAGNITUDE_COMPONENTS_METRICS_HPP
#define MAGNITUDE_COMPONENTS_METRICS_HPP
#include <definitions.hpp>
#include <components/component.hpp>
#include <imgui/imgui.h>

#define METRICS_FRAME_TIME_BUFFER_SIZE 512

class MetricsComponent : public EditorComponent
{

    public:
                         MetricsComponent(i32 index, std::string name);
        virtual         ~MetricsComponent();

        virtual void     render()    override;
        virtual void     update()    override;

        void             set_frame_time(r32 delta);

    protected:
        bool pause          = false;

        r32 frame_time      = 0.0f;
        r32 frame_average   = 0.0f;
        i32 frame_index     = 0;
        r32 frame_time_set[METRICS_FRAME_TIME_BUFFER_SIZE];

};

#endif
