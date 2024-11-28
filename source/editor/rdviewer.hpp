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
                            RDViewerComponent(i32 id, std::string name);
        virtual            ~RDViewerComponent();

        virtual void        render()    override;
        virtual bool        close()     override;
        virtual bool        open()      override;

        bool                new_file();
        bool                close_file();
        bool                set_file(Filepath path);
        bool                save_file();
        bool                save_as_file();
        bool                file_unsaved() const;
        Filepath            get_file_path() const;

    protected:
        Filepath        file_path;
        TextEditor      file_editor;
        bool            file_changes;
};

#endif
