#ifndef MAGNITUDE_COMPONENTS_TEXTEDITOR_HPP
#define MAGNITUDE_COMPONENTS_TEXTEDITOR_HPP
#include <definitions.hpp>
#include <components/component.hpp>

#include <utilities/resourceman.hpp>
#include <rdparser/rdparser.hpp>
#include <utilities/path.hpp>

#include <imgui/imgui.h>
#include <balazedit/texteditor.h>

class TextEditorComponent : public EditorComponent
{

    public:
                            TextEditorComponent(i32 id, std::string name);
        virtual            ~TextEditorComponent();

        virtual void        render()    override;
        virtual void        update()    override;

        bool                new_file();
        bool                close_file();
        bool                set_file(Filepath path);
        bool                save_file();
        bool                save_as_file();
        bool                file_unsaved() const;
        Filepath            get_file_path() const;

        bool                is_parse_valid() const;
        RDSyntaxParser*     get_parser();

    protected:
        bool                validate_and_parse();

    protected:
        RDSyntaxParser  parser;

        Filepath        file_path;
        TextEditor      file_editor;
        bool            file_changes;
};

#endif
