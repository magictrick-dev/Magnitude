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
    
    // --- Runtime Configuration & Main Loop ----------------------------------- 
    //
    // Launch the window, perform the operation(s). We construct a window, then
    // establish an OpenGL context. This ensures that everything is properly set
    // up for hardware rendering and DearImGUI.
    //
    // The general workflow is as follows:
    //      1.  Create the window.
    //      2.  Establish a render context (probably OpenGL).
    //      3.  Show window.
    //      4.  Enter runtime loop.
    //      5.  Bind our render context to the active window (probably main window).
    //      6.  Begin frame in render context.
    //      7.  Poll window events.
    //      8.  Do some rendering & logic updates.
    //      9.  End frame in render context.
    //      10. Unbind the render context.
    //      11. Swap the buffers in the window.
    //

    // Create the window and attempt to establish an OpenGL render context.
    std::shared_ptr<Window> main_window = Window::create("Magnitude", 1280, 720);
    OpenGLRenderContext::create_render_context(main_window);
    
    // Initialize OpenGL.
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Preset values, swap frame afterwards to show it.
    glViewport(0, 0, main_window->get_width(), main_window->get_height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    main_window->swap_frames();
    main_window->show();

    // Setup the editor and the necessary components we want to use.
    Editor& editor = Editor::get();
    editor.add_component<MetricsComponent>("metrics");
    editor.add_component<RDViewerComponent>("rdviewer");

    // Set the file, if possible.
    auto rdviewer = editor.get_component_by_name<RDViewerComponent>("rdviewer");
    MAG_ENSURE_PTR(rdviewer);
    rdviewer->set_file(runtime_path);

    while (!main_window->should_close())
    {

        // Set the context.
        OpenGLRenderContext::bind(main_window);

        // Poll the window events.
        main_window->poll_events();
        
        // Begin the rendering.
        OpenGLRenderContext::begin_frame();
        glViewport(0, 0, main_window->get_width(), main_window->get_height());
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Render the editor.
        ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        editor.render();
        ImGui::PopStyleColor();

        static bool show_demo = false;
        if (ImGui::IsKeyPressed(ImGuiKey_F3))
        {
            show_demo = !show_demo;
        }
        if (show_demo)
        {
            ImGui::ShowDemoWindow(&show_demo);
        }

        // End the rendering.
        OpenGLRenderContext::end_frame();
        OpenGLRenderContext::unbind();
        main_window->swap_frames();

    }

    return 0;
}
