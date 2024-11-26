#ifndef MAGNITUDE_UTILITIES_LOGGING_HPP
#define MAGNITUDE_UTILITIES_LOGGING_HPP
#include <deque>
#include <vector>
#include <string>
#include <definitions.hpp>

#define LOGGER_ROLLING_BUFFER_SIZE 1024

// --- Logger ------------------------------------------------------------------
//
// The logger is designed to be inspected by the UI as well as output well-formatted
// messages to standard output (if enabled). By default, only critical and error flags
// are put into standard output. All logs are viewable in the UI and are filterable.
// The logger itself is not thread-safe, so therefore the logger will need to be
// adapted to make it thread safe.
//
// A log can be classified with any amount of flags (though it wouldn't make sense
// to set a log as two different levels... but you *could* technically do that).
// They're primarily used for filtering and such.
//
// For simple logging, just use the log_[level]() functions and pass in NULL, 0,
// or LogFlags::LogNone as the first parameter, along with the message.
//
// The rolling buffer size basically keeps track of how many logs are allowed to
// stay in memory at once. Since C++ STL is kinda greedy, we reserve this amount
// to be a fixed constant LOGGER_ROLLING_BUFFER_SIZE. This can be adjusted more or
// less depending on how much memory you actually want to use for this.
//

typedef u32 LogFlags;
#define LogFlag_None        (0)
#define LogFlag_Debug       (1 << 0)
#define LogFlag_Info        (1 << 1)
#define LogFlag_Warning     (1 << 2)
#define LogFlag_Critical    (1 << 3)
#define LogFlag_Error       (1 << 4)
#define LogFlag_Internal    (1 << 16)
#define LogFlag_Renderer    (1 << 17)
#define LogFlag_Parser      (1 << 18)

struct LogDescriptor 
{
    std::string     message;
    u32             flags;
};

class Logger
{

    public:
        static std::vector<LogDescriptor> get_messages();
        static std::vector<LogDescriptor> get_messages(i32 amount);
        static std::vector<LogDescriptor> get_messages_and_filter(u32 filter);
        static std::vector<LogDescriptor> get_messages_and_filter(u32 filter, i32 amount);

        static void log(u32 flags, std::string message);
        static void log_debug(u32 flags, std::string message);
        static void log_info(u32 flags, std::string message);
        static void log_warning(u32 flags, std::string message);
        static void log_critical(u32 flags, std::string message);
        static void log_error(u32 flags, std::string message);

        virtual        ~Logger();
    protected:
                                Logger();
        static Logger&          get();


    protected:
        std::deque<LogDescriptor> rolling_buffer;

};

#endif
