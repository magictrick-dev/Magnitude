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


std::vector<LogDescriptor> Logger::
get_messages()
{

    return {};
}

std::vector<LogDescriptor> Logger::
get_messages(i32 amount)
{

    return {};
}

std::vector<LogDescriptor> Logger::
get_messages_and_filter(LogFlags filter)
{

    return {};
}

std::vector<LogDescriptor> Logger::
get_messages_and_filter(LogFlags filter, i32 amount)
{

    return {};
}

void Logger::
log(LogFlags flags, std::string message)
{

    return;
}

void Logger::
log_debug(LogFlags flags, std::string message)
{

    return;
}

void Logger::
log_info(LogFlags flags, std::string message)
{

    return;
}

void Logger::
log_warning(LogFlags flags, std::string message)
{

    return;
}

void Logger::
log_critical(LogFlags flags, std::string message)
{

    return;
}

void Logger::
log_error(LogFlags flags, std::string message)
{

    return;
}

