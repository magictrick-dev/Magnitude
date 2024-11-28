// --- Magnitude ---------------------------------------------------------------
//
// Developed by Christopher DeJong | November 2024 - Current
//
//      Magnitude is a graphics development library built from scratch which
//      incorporates techniques and algorithms introduced from a comp. science
//      course at Northern Illinois University.
//
//      If you're reading this code then either you're interested in computer
//      graphics OR you're an employer looking over my pet projects. If you're
//      the latter, let me preface that this entire project is the culmination
//      of *years* of self-study, trial and error, and optimizations made trying
//      to make a general purpose framework to experiment with. This is *not* a
//      game engine (I have a separate project for that) and it is not designed
//      to be wholly interactive. If you're looking to play around or test some
//      hand crafted scene files, please look in the rdviews folder and run the
//      p[integer].rd files (not the s[integer].rd files, they're assignment files
//      that probably aren't going to work with this current system).
//
// -----------------------------------------------------------------------------
#include <iostream>
#include <definitions.hpp>

#include <platform/window.hpp>
#include <platform/opengl.hpp>
#include <platform/system.hpp>
#include <platform/input.hpp>

#include <utilities/path.hpp>
#include <utilities/cli.hpp>
#include <utilities/resourceman.hpp>
#include <utilities/rdtokenizer.hpp>
#include <utilities/logging.hpp>

#include <graphics/color.hpp>
#include <graphics/bitmap.hpp>

#include <editor/editor.hpp>
#include <editor/metrics.hpp>
#include <editor/rdviewer.hpp>
#include <editor/mainmenu.hpp>
#include <editor/sceneviewer.hpp>
#include <editor/console.hpp>
#include <editor/inspector.hpp>
#include <editor/imguidocs.hpp>

#include <imgui/imgui.h>
#include <glad/glad.h>

i32
main(i32 argc, cptr *argv)
{

    // --- Command Line Parsing ------------------------------------------------
    //
    // Parse the command line arguments and ensure we have a valid starting state.
    // Otherwise, fast-exit and print help docs.
    //

    if (!CLI::parse(argc, argv))
    {
        std::cout << "[ CLI ] Unable to parse the given command line arguments." << std::endl;
        CLI::short_help();
        return -1;
    }
    
    // Get the rdview filepath.
    Filepath runtime_path;
    if (CLI::size() >= 2)
    {
        CLIArgument *initial_argument = CLI::get(1);
        if (initial_argument->get_type() == CLIArgumentType::String)
        {
            CLIValue *user_argument = dynamic_cast<CLIValue*>(initial_argument);
            MAG_ASSERT(user_argument != nullptr); // This should never occur.
            ccptr user_string = user_argument->get_string();
            runtime_path = Filepath::cwd();
            runtime_path += "./";
            runtime_path += user_string;
            runtime_path.canonicalize();
        }

        if (!runtime_path.is_valid_file())
        {
            Logger::log_warning(LogFlag_None, "Unable to open the file: %s, file does not exist.",
                    runtime_path.c_str());
            runtime_path = "";
        }

    }
    
    // --- Runtime Configuration -----------------------------------------------
    //
    // Generating a functional runtime environment requires a standard order
    // of operations to ensure everything gets boiled together correctly.
    // The window and OpenGL context must be created first. Then, it would make
    // sense to properly initialize OpenGL afterwards.
    //
    // The application using editor components as its primary method of delegating
    // functionality across the UI. There is coupling in functionality. Certain
    // components can look for other components and interact with them. Therefore
    // all components must be added to the component list.
    //
    // Afterwards, the runtime loop is run. So long as all components are added
    // and successfully exist, there shouldn't be order errors in the runtime
    // loop. If there are, it is because there is tight coupling of components.
    //

    // Create the window and attempt to establish an OpenGL render context.
    std::shared_ptr<Window> main_window = Window::create("Magnitude", 1600, 900);
    OpenGLRenderContext::create_render_context(main_window);
    
    // Initialize OpenGL.
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, main_window->get_width(), main_window->get_height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Setup the editor and the necessary components we want to use.
    Editor::add_component<MainMenuComponent>("main_menu");
    Editor::add_component<SceneViewerComponent>("Scene Viewer");
    Editor::add_component<InspectorComponent>("Inspector");
    Editor::add_component<MetricsComponent>("Metrics");
    Editor::add_component<RDViewerComponent>("Editor");
    Editor::add_component<ConsoleComponent>("Console");
    Editor::add_component<ImguiDocsComponent>("Imgui Documentation");

    // Set the file, if possible.
    auto rdviewer = Editor::get_component_by_name<RDViewerComponent>("Editor");
    auto metrics = Editor::get_component_by_name<MetricsComponent>("Metrics");
    rdviewer->set_file(runtime_path);

    // Show the window and begin the runtime.
    main_window->swap_frames();
    main_window->show();

    u64 frame_begin = 0;
    u64 frame_end   = 0;
    r32 delta_time  = 1.0f / 60.0f;

    while (!main_window->should_close())
    {

        frame_begin = system_timestamp();
        main_window->poll_events();
        metrics->set_frame_time(delta_time);

        // Start the frame.
        OpenGLRenderContext::bind(main_window);
        OpenGLRenderContext::begin_frame();
        Editor::update();
        Editor::render();

        // Begin the rendering outside the renderer.
        glViewport(0, 0, main_window->get_width(), main_window->get_height());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // End the rendering.
        OpenGLRenderContext::end_frame();
        OpenGLRenderContext::unbind();
        main_window->swap_frames();

        frame_end = system_timestamp();
        delta_time = system_timestamp_difference_ss(frame_begin, frame_end);

    }

    return 0;
}
