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
#include <graphics/color.hpp>
#include <graphics/bitmap.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>
#include <glad/glad.h>
#include <balazedit/texteditor.h>

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
        return -1;
    }
    
    if (CLI::size() <= 1)
    {
        CLI::short_help();
        return 0;
    }

    // Get the rdview filepath.
    Filepath runtime_path = Filepath::cwd();
    CLIArgument *initial_argument = CLI::get(1);
    if (initial_argument->get_type() == CLIArgumentType::String)
    {
        CLIValue *user_argument = dynamic_cast<CLIValue*>(initial_argument);
        MAG_ASSERT(user_argument != nullptr); // This should never occur.
        ccptr user_string = user_argument->get_string();
        runtime_path += "./";
        runtime_path += user_string;
        runtime_path.canonicalize();
    }

    // Attempt to load the user file into the resource manager.
    rhandle user_file_handle = ResourceManager::create_file_resource(runtime_path);
    MAG_ASSERT(ResourceManager::resource_handle_is_valid(user_file_handle) &&
            "This shouldn't fail, we expressly check for this at startup.");
    MAG_ASSERT(ResourceManager::load_resource(user_file_handle) &&
            "This shouldn't fail, we haven't even loaded the resource yet.");

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
    std::shared_ptr<Window> main_window = Window::create("Magnitude Graphics Visualizer", 1280, 720);
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

    // Create the text editor.
    TextEditor basic_editor;
    basic_editor.SetText(ResourceManager::get_resource_as_string(user_file_handle));

    // Tokenizer.
    RDViewTokenizer tokenizer(runtime_path);
    RDViewToken current_token = tokenizer.get_current();
    ResourceManager::release_resource(user_file_handle); // No longer need it.

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
        editor.render();

        // Show the Dear ImGUI demo window.
        static bool show_demo = true;
        if (show_demo)
        {
            ImGui::ShowDemoWindow(&show_demo);
        }

        // Show the editor.
        static bool show_editor = true;
        if (show_editor)
        {
            ImGui::Begin("RDView Text Editor", &show_editor);
            basic_editor.Render("EditorArea");
            ImGui::End();
        }

        // End the rendering.
        OpenGLRenderContext::end_frame();
        OpenGLRenderContext::unbind();
        main_window->swap_frames();

    }

    return 0;
}
