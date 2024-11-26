#include <utilities/logging.hpp>

Logger::
Logger()
{

}

Logger::
~Logger()
{

}

Logger& Logger::
get()
{

    static std::shared_ptr<Logger> instance = nullptr;
    if (instance == nullptr)
    {

        Logger *logger = new Logger();
        instance = std::shared_ptr<Logger>(logger);

    }

    return *instance;

}


