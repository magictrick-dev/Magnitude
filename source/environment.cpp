#include <environment.hpp>

// --- Environment -------------------------------------------------------------

Environment::
Environment()
{

}

Environment& Environment::
get()
{

    static Environment self;
    return self;

}
