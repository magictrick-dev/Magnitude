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
#include <utilities/path.hpp>
#include <utilities/cli.hpp>

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
    // Launch the window, perform the operation(s).
    //

    // Create the window.
    std::shared_ptr<Window> main_window = Window::create("Example Project", 600, 400);
    main_window->show();

    while (!main_window->should_close())
    {

        main_window->poll_events();

        if (main_window->did_size_change())
        {
            i32 new_width = main_window->get_width();
            i32 new_height = main_window->get_height();
            std::cout << "The currently active window's size change to: "
                << new_width << ", " << new_height << "." << std::endl;
        }

        if (main_window->did_focus_change())
        {
            bool focused = main_window->is_focused();
            std::cout << "The window's focus changed: " << focused << "." << std::endl;
        }

        main_window->swap_frames();

    }

    return 0;
}
