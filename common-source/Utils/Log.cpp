#include "Log.h"

std::vector<std::ostream*> log_streams_by_level[LogLevelMax];
LogLevel           min_log_level = INFO;
sf::Mutex          global_log_mutex;
std::string        log_prefix_format;
