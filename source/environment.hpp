#ifndef MAGNITUDE_ENVIRONMENT_HPP
#define MAGNITUDE_ENVIRONMENT_HPP
#include <definitions.hpp>
#include <utilities/path.hpp>
#include <utilities/logging.hpp>
#include <utilities/resourceman.hpp>
#include <platform/filesystem.hpp>
#include <editor/editor.hpp>
#include <editor/rdviewer.hpp>

class Environment
{
    
    public:
        static inline bool              rdview_file_open();
        static inline bool              rdview_file_save(Filepath location);
        static inline bool              rdview_file_save_as(Filepath location);
        static inline bool              rdview_file_set(Filepath path);

    protected:
        static inline Filepath          rdview_file     = "";

};

bool Environment::
rdview_file_set(Filepath path)
{

    if (!path.is_valid_file()) return false;

    Environment::rdview_file = path;
    Editor& editor = Editor::get();
    std::shared_ptr<RDViewerComponent> viewer = editor.get_component_by_name<RDViewerComponent>("Editor");
    MAG_ASSERT(viewer != nullptr);
    viewer->set_file(path);

    return true;

}

bool Environment::
rdview_file_open()
{

    std::string result = file_open_system_dialogue();
    if (result.empty()) return false;

    Filepath user_path = result.c_str();
    if (!user_path.is_valid_file())
    {
        Logger::log_error(LogFlag_None, "Unable to open file %s, unknown reason.",
                user_path.c_str());
        return false;
    }
    
    Environment::rdview_file_set(user_path);
    return true;

}

bool Environment::
rdview_file_save(Filepath location)
{

    return false;
}

bool Environment::
rdview_file_save_as(Filepath location)
{

    return false;
}

#endif
