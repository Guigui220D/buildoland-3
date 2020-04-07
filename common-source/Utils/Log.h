#pragma once

#include <ctime>

#include <ostream>
#include <vector>

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

#include "../../external/fmt/include/fmt/core.h"
#include "../../external/fmt/include/fmt/ostream.h"

enum LogLevel
{
    INFO,
    WARN,
    ERROR,

    LogLevelMax
};

extern std::vector<std::ostream*> log_streams_by_level[LogLevelMax];
extern LogLevel           min_log_level;
extern sf::Mutex          global_log_mutex;
extern std::string        log_prefix_format;

template <typename... Args>
void log(LogLevel level, const char* fmt, Args&&... args)
{
    sf::Lock lock(global_log_mutex);

    time_t rawtime;
    struct tm * timeinfo;
    char date[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(date,sizeof(date),"%d-%m-%Y %H:%M:%S",timeinfo);

    if (level >= min_log_level)
        for (const auto& stream : log_streams_by_level[level])
        {
            fmt::print(*stream, log_prefix_format, fmt::arg("date", date));
            fmt::print(*stream, fmt, std::forward<Args>(args)...);
        }
}
