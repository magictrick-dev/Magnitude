#ifndef MAGNITUDE_UTILITIES_LOGGING_HPP
#define MAGNITUDE_UTILITIES_LOGGING_HPP
#include <definitions.hpp>

class Logger
{

    public:
        static Logger&          get();



        virtual        ~Logger();
    protected:
                        Logger();

};

#endif
