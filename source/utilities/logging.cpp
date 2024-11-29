#include <iostream>
#include <format>
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

    Logger& self = Logger::get();

    std::vector<LogDescriptor> logs;
    for (i32 i = 0; i < self.rolling_buffer.size(); ++i)
    {
        logs.push_back(self.rolling_buffer[i]);
    }

    return logs;

}

std::vector<LogDescriptor> Logger::
get_messages(i32 amount)
{

    Logger& self = Logger::get();

    std::vector<LogDescriptor> logs;
    i32 count = 0;
    for (i32 i = 0; i < self.rolling_buffer.size(); ++i)
    {
        logs.push_back(self.rolling_buffer[i]);
        count++;
        if (count == amount) break;
    }

    return logs;

}

std::vector<LogDescriptor> Logger::
get_messages_and_filter(LogFlags filter)
{

    Logger& self = Logger::get();
    std::vector<LogDescriptor> logs;
    for (i32 i = 0; i < self.rolling_buffer.size(); ++i)
    {
        if (self.rolling_buffer[i].flags & filter)
            logs.push_back(self.rolling_buffer[i]);
    }

    return logs;

}

std::vector<LogDescriptor> Logger::
get_messages_and_filter(LogFlags filter, i32 amount)
{

    Logger& self = Logger::get();
    std::vector<LogDescriptor> logs;
    i32 count = 0;
    for (i32 i = 0; i < self.rolling_buffer.size(); ++i)
    {
        if (self.rolling_buffer[i].flags & filter)
            logs.push_back(self.rolling_buffer[i]);
        count++;
        if (count == amount) break;
    }

    return logs;

}

