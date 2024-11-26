#include <iostream>
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
    for (i32 i = self.rolling_buffer.size() - 1; i >= 0; --i)
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
    for (i32 i = self.rolling_buffer.size() - 1; i >= 0; --i)
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
    for (i32 i = self.rolling_buffer.size() - 1; i >= 0; --i)
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
    for (i32 i = self.rolling_buffer.size() - 1; i >= 0; --i)
    {
        if (self.rolling_buffer[i].flags & filter)
            logs.push_back(self.rolling_buffer[i]);
        count++;
        if (count == amount) break;
    }

    return logs;

}

void Logger::
log(LogFlags flags, std::string message)
{

    if (flags & (LogFlag_Critical | LogFlag_Warning | LogFlag_Error))
    {
        std::cout << message << std::endl;
    }

    Logger& self = Logger::get();
    self.rolling_buffer.push_back({message, flags});
    if (self.rolling_buffer.size() >= 1024) self.rolling_buffer.pop_front();
    return;

}

void Logger::
log_debug(LogFlags flags, std::string message)
{

    Logger& self = Logger::get();
    self.rolling_buffer.push_back({message, flags | LogFlag_Debug});
    if (self.rolling_buffer.size() >= 1024) self.rolling_buffer.pop_front();
    return;

}

void Logger::
log_info(LogFlags flags, std::string message)
{

    Logger& self = Logger::get();
    self.rolling_buffer.push_back({message, flags | LogFlag_Info});
    if (self.rolling_buffer.size() >= 1024) self.rolling_buffer.pop_front();
    return;

}

void Logger::
log_warning(LogFlags flags, std::string message)
{

    std::cout << message << std::endl;
    Logger& self = Logger::get();
    self.rolling_buffer.push_back({message, flags | LogFlag_Warning});
    if (self.rolling_buffer.size() >= 1024) self.rolling_buffer.pop_front();
    return;

}

void Logger::
log_critical(LogFlags flags, std::string message)
{

    std::cout << message << std::endl;
    Logger& self = Logger::get();
    self.rolling_buffer.push_back({message, flags | LogFlag_Critical});
    if (self.rolling_buffer.size() >= 1024) self.rolling_buffer.pop_front();
    return;

}

void Logger::
log_error(LogFlags flags, std::string message)
{

    std::cout << message << std::endl;
    Logger& self = Logger::get();
    self.rolling_buffer.push_back({message, flags | LogFlag_Error});
    if (self.rolling_buffer.size() >= 1024) self.rolling_buffer.pop_front();
    return;

}

