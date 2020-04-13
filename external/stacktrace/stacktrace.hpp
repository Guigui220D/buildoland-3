#ifndef STACKTRACE_HPP
#define STACKTRACE_HPP

#include <cstdint>

void init_stack_trace_handler(int argc, char* argv[]);

struct stack_frame
{
    uintptr_t addr { 0 };
    char func_name[128] = { 0 };
    char file_name[128] = { 0 };
    int         file_line { 0 };
};

#endif // STACKTRACE_HPP
