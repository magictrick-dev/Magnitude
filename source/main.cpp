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
#include <graphics/color.hpp>
#include <graphics/bitmap.hpp>
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

    std::cout << "[ CLI ] View file from arguments is: " << runtime_path << std::endl;

    // --- Runtime Configuration & Main Loop ----------------------------------- 
    //
    // Launch the window, perform the operation(s). We construct a window, then
    // establish an OpenGL context. This ensures that everything is properly set
    // up for hardware rendering and DearImGUI.
    //

    // Create the window and attempt to establish an OpenGL render context.
    std::shared_ptr<Window> main_window = Window::create("Example Project", 1280, 720);
    OpenGLRenderContext::create(main_window);
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

    // Create a bitmap that we will draw to. (It's optimal.)
    //BitmapImageWrapper render_bitmap(main_window->get_width(), main_window->get_height());

    while (!main_window->should_close())
    {

        // Poll the window events.
        main_window->poll_events();

        /*
        // If the window size changed, we need to resize our render bitmap.
        if (main_window->did_size_change())
        {
            render_bitmap.resize(main_window->get_width(), main_window->get_height());
        }

        // Render to the window.
        for (i32 y = 0; y < main_window->get_height(); ++y)
        {

            for (i32 x = 0; x < main_window->get_width(); ++x)
            {

                r32 red     = (r32)x / main_window->get_width();
                r32 green   = (r32)y / main_window->get_height();
                r32 blue    = 0.0f;
                r32 alpha   = 1.0f;

                RGBAColor assignment_color(red, green, blue, alpha);
                packed_color output_color = assignment_color.pack_to_bgra();
                render_bitmap.set_pixel(x, y, output_color);

            }

        }

        // Take our bitmap and render it to the window.
        bitmap_image image_format = render_bitmap.get_image_format();
        main_window->set_bitmap(0, 0, &image_format.info_header, render_bitmap.get_data());
        */

        glViewport(0, 0, main_window->get_width(), main_window->get_height());
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Swap the frames.
        main_window->swap_frames();

    }

    return 0;
}
