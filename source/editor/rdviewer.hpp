#ifndef MAGNITUDE_EDITOR_RDVIEWER_HPP
#define MAGNITUDE_EDITOR_RDVIEWER_HPP
#include <definitions.hpp>
#include <editor/component.hpp>

#include <utilities/resourceman.hpp>
#include <utilities/path.hpp>

#include <imgui/imgui.h>
#include <balazedit/texteditor.h>

class RDViewerComponent : public EditorComponent
{

    public:
        inline                  RDViewerComponent(i32 id, std::string name);
        inline virtual         ~RDViewerComponent();

        inline virtual void     render()    override;
        inline virtual bool     close()     override;
        inline virtual bool     open()      override;

        inline bool             set_file(Filepath path);

    protected:
        Filepath        file_path;
        TextEditor      file_editor;
};

inline RDViewerComponent::
RDViewerComponent(i32 id, std::string name) : EditorComponent(id, name)
{

    this->visible = true;

}

inline RDViewerComponent::
~RDViewerComponent()
{

}

inline bool RDViewerComponent::
close()
{

    this->visible = false;
    return true;

}

inline bool RDViewerComponent::
open()
{

    this->visible = true;
    return true;

}

inline void RDViewerComponent::
render()
{

    if (this->visible == false) return;
    
    ImGui::Begin("Text Editor", &this->visible);
    this->file_editor.Render("Text Editor");
    ImGui::End();

}

inline bool RDViewerComponent::
set_file(Filepath path)
{

    MAG_ASSERT(path.is_valid_file() && "This should be checked prior.");
    this->file_path = path;

    rhandle resource = ResourceManager::create_file_resource(path);
    if (!ResourceManager::resource_handle_is_valid(resource)) return false;
    if (!ResourceManager::load_resource(resource)) return false;

    this->file_editor.SetText(ResourceManager::get_resource_as_string(resource));

    return true;

}

#endif
