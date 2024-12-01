#ifndef MAGNITUDE_COMPONENTS_SCENEVIEWER_HPP
#define MAGNITUDE_COMPONENTS_SCENEVIEWER_HPP
#include <definitions.hpp>
#include <components/component.hpp>
#include <renderer/framebuffer.hpp>

class SceneViewerComponent : public EditorComponent
{

    public:
                         SceneViewerComponent(i32 id, std::string name);
        virtual         ~SceneViewerComponent();

        virtual void     render()    override;
        virtual void     update()    override;

    protected:
        Framebuffer framebuffer;
        i32 viewer_width = 100;
        i32 viewer_height = 100;

        bool static_only        = false;
        bool fixed_aspect_ratio = true;

    protected:
        u32 vtx_buffer;
        u32 prog;

};

#endif
