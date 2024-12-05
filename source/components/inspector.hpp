#ifndef MAGNITUDE_COMPONENTS_INSPECTOR_HPP
#define MAGNITUDE_COMPONENTS_INSPECTOR_HPP
#include <definitions.hpp>
#include <components/component.hpp>
#include <imgui/imgui.h>
#include <renderer/pipeline.hpp>
#include <rdparser/rdparser.hpp>

class InspectorComponent : public EditorComponent
{

    public:
                         InspectorComponent(i32 id, std::string name);
        virtual         ~InspectorComponent();

        virtual void     render()    override;
        virtual void     update()    override;

    protected:
        void    render_camera_properties(RDSyntaxNodeFrame *current_frame);

};

#endif
