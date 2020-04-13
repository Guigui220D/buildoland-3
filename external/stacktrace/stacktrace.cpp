
/* compile with:
on linux:   gcc -g stack_traces.c
on OS X:    gcc -g -fno-pie stack_traces.c
on windows: gcc -g stack_traces.c -limagehlp
*/

#include "stacktrace.hpp"

#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <string>

#include "../../common-source/Utils/Log.h"

#ifdef _WIN32
#include <windows.h>
#include <imagehlp.h>
#undef ERROR
#else
#include <err.h>
#include <execinfo.h>
#endif


static char const * icky_global_program_name;
void set_signal_handler();
void init_stack_trace_handler(int argc, char* argv[])
{
    icky_global_program_name = argv[0];
    set_signal_handler();
}

/* Resolve symbol name and source location given the path to the executable
   and an address */
stack_frame addr2line(char const * const program_name, void const * const addr)
{
    stack_frame frame;
    frame.addr = (uintptr_t)addr;

    char addr2line_cmd[512] = {0};

/* have addr2line map the address to the relent line in the code */
#ifdef __APPLE__
    /* apple does things differently... */
    sprintf(addr2line_cmd,"atos -o \"%.256s\" %p", program_name, addr);
#else
    sprintf(addr2line_cmd,"addr2line -C -f -e \"%.256s\" %p", program_name, addr);
#endif

    /* Open the command for reading. */
    FILE* fp = popen(addr2line_cmd, "r");
    if (fp == NULL) {
        return frame;
    }

    // split the return string to get the function name, file and line number info
    char buffer[1024];
    // first line : function name
    fgets(buffer, 1024, fp);
    strcpy(frame.func_name, buffer);
    frame.func_name[strlen(frame.func_name)-1] = '\0'; // remove end-of-line character
    // second line : file:line
    fgets(buffer, 1024, fp);
    const char* colon_ptr = strrchr(buffer, ':');
    memcpy(frame.file_name, buffer, colon_ptr - buffer);
    frame.file_name[colon_ptr - buffer] = '\0';
    frame.file_line = strtol(colon_ptr+1, NULL, 10);

    /* close */
    pclose(fp);

    return frame;
}


#ifdef _WIN32

stack_frame windows_get_frame_info(STACKFRAME* frame)
{
    stack_frame ret;

    DWORD64 functionAddress;
    unsigned int file_line = 0;

    ret.addr = frame->AddrPC.Offset;

    DWORD moduleBase = SymGetModuleBase(GetCurrentProcess(), frame->AddrPC.Offset);
    char moduleBuff[MAX_PATH];
    if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduleBuff , MAX_PATH))
    {
    }

    char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
    PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
    symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) + 255;
    symbol->MaxNameLength = 254;

    if (SymGetSymFromAddr(GetCurrentProcess(), frame->AddrPC.Offset, NULL, symbol))
    {
        memcpy(ret.func_name, symbol->Name, strlen(symbol->Name));
    }

    DWORD  offset = 0;
    IMAGEHLP_LINE line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

    if (SymGetLineFromAddr(GetCurrentProcess(), frame->AddrPC.Offset, &offset, &line))
    {
        memcpy(ret.file_name, line.FileName, strlen(line.FileName));
        file_line = line.LineNumber;
    }

    return ret;
}

void windows_print_stacktrace(CONTEXT* context)
{
    SymInitialize(GetCurrentProcess(), 0, true);

    STACKFRAME win_frame = { 0 };

#if _WIN64
    DWORD machine = IMAGE_FILE_MACHINE_AMD64;
    /* setup initial stack frame */
    win_frame.AddrPC.Offset         = context->Rip;
    win_frame.AddrPC.Mode           = AddrModeFlat;
    win_frame.AddrStack.Offset      = context->Rsp;
    win_frame.AddrStack.Mode        = AddrModeFlat;
    win_frame.AddrFrame.Offset      = context->Rbp;
    win_frame.AddrFrame.Mode        = AddrModeFlat;
#elif defined(_WIN32)
    DWORD machine = IMAGE_FILE_MACHINE_I386;
    /* setup initial stack frame */
    win_frame.AddrPC.Offset         = context->Eip;
    win_frame.AddrPC.Mode           = AddrModeFlat;
    win_frame.AddrStack.Offset      = context->Esp;
    win_frame.AddrStack.Mode        = AddrModeFlat;
    win_frame.AddrFrame.Offset      = context->Ebp;
    win_frame.AddrFrame.Mode        = AddrModeFlat;
#else
    return; // unsupported platform
#endif

    std::string total_output;

    log(ERROR, "Signal caught, stack frame :\n");

    while (StackWalk(machine ,
                     GetCurrentProcess(),
                     GetCurrentThread(),
                     &win_frame,
                     context,
                     0,
                     SymFunctionTableAccess,
                     SymGetModuleBase,
                     0 ) )
    {
        stack_frame frame;
#ifdef __MINGW32__
        frame = addr2line(icky_global_program_name, (void*)win_frame.AddrPC.Offset);
#else
        frame = windows_get_frame_info(&win_frame);
#endif
        log(ERROR, "{}: Func {}, File {}, line {}\n", (void*)frame.addr, frame.func_name, frame.file_name, frame.file_line);

    }
    SymCleanup( GetCurrentProcess() );
}

LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
{
    switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            log(ERROR, "Error: EXCEPTION_ACCESS_VIOLATION\n");
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            log(ERROR, "Error: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n");
            break;
        case EXCEPTION_BREAKPOINT:
            return EXCEPTION_CONTINUE_SEARCH;
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            log(ERROR, "Error: EXCEPTION_DATATYPE_MISALIGNMENT\n");
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            return EXCEPTION_CONTINUE_SEARCH;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            log(ERROR, "Error: EXCEPTION_FLT_DIVIDE_BY_ZERO\n");
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            log(ERROR, "Error: EXCEPTION_FLT_INEXACT_RESULT\n");
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            log(ERROR, "Error: EXCEPTION_FLT_INVALID_OPERATION\n");
            break;
        case EXCEPTION_FLT_OVERFLOW:
            log(ERROR, "Error: EXCEPTION_FLT_OVERFLOW\n");
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            log(ERROR, "Error: EXCEPTION_FLT_STACK_CHECK\n");
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            log(ERROR, "Error: EXCEPTION_FLT_UNDERFLOW\n");
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            log(ERROR, "Error: EXCEPTION_ILLEGAL_INSTRUCTION\n");
            break;
        case EXCEPTION_IN_PAGE_ERROR:
            log(ERROR, "Error: EXCEPTION_IN_PAGE_ERROR\n");
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            log(ERROR, "Error: EXCEPTION_INT_DIVIDE_BY_ZERO\n");
            break;
        case EXCEPTION_INT_OVERFLOW:
            log(ERROR, "Error: EXCEPTION_INT_OVERFLOW\n");
            break;
        case EXCEPTION_INVALID_DISPOSITION:
            log(ERROR, "Error: EXCEPTION_INVALID_DISPOSITION\n");
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            log(ERROR, "Error: EXCEPTION_NONCONTINUABLE_EXCEPTION\n");
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            log(ERROR, "Error: EXCEPTION_PRIV_INSTRUCTION\n");
            break;
        case EXCEPTION_SINGLE_STEP:
            return EXCEPTION_CONTINUE_SEARCH;
        case EXCEPTION_STACK_OVERFLOW:
            log(ERROR, "Error: EXCEPTION_STACK_OVERFLOW\n");
            break;
        default:
            log(ERROR, "Error: Unrecognized Exception\n");
            break;
    }
    fflush(stderr);
    /* If this is a stack overflow then we can't walk the stack, so just show
      where the error happened */
    if (EXCEPTION_STACK_OVERFLOW != ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
        windows_print_stacktrace(ExceptionInfo->ContextRecord);
    }
    else
    {
        addr2line(icky_global_program_name, (void*)ExceptionInfo->ContextRecord->Rip);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

void set_signal_handler()
{
    SetUnhandledExceptionFilter(windows_exception_handler);
}
#else

#define MAX_STACK_FRAMES 64
static void *stack_traces[MAX_STACK_FRAMES];
void posix_print_stack_trace()
{
    int i, trace_size = 0;
    char **messages = (char **)NULL;

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    messages = backtrace_symbols(stack_traces, trace_size);

    /* skip the first couple stack frames (as they are this function and
     our handler) and also skip the last frame as it's (always?) junk. */
    // for (i = 3; i < (trace_size - 1); ++i)
    for (i = 0; i &lt; trace_size; ++i) // we'll use this for now so you can see what's going on
    {
        stack_frame frame;
        frame = addr2line(icky_global_program_name, stack_traces[i]);
        log(ERROR, "{}: Func {}, File {}, line {}\n", (void*)frame.addr, frame.func_name, frame.file_name, frame.file_line);
    }
    if (messages) { free(messages); }
  }

void posix_signal_handler(int sig, siginfo_t *siginfo, void *context)
{
    (void)context;
    switch(sig)
    {
        case SIGSEGV:
            log(ERROR, "Caught SIGSEGV: Segmentation Fault\n");
            break;
        case SIGINT:
            log(ERROR, "Caught SIGINT: Interactive attention signal, (usually ctrl+c)\n");
            break;
        case SIGFPE:
            switch(siginfo->si_code)
            {
                case FPE_INTDIV:
                    log(ERROR, "Caught SIGFPE: (integer divide by zero)\n");
                    break;
                case FPE_INTOVF:
                    log(ERROR, "Caught SIGFPE: (integer overflow)\n");
                    break;
                case FPE_FLTDIV:
                    log(ERROR, "Caught SIGFPE: (floating-point divide by zero)\n");
                    break;
                case FPE_FLTOVF:
                    log(ERROR, "Caught SIGFPE: (floating-point overflow)\n");
                    break;
                case FPE_FLTUND:
                    log(ERROR, "Caught SIGFPE: (floating-point underflow)\n");
                    break;
                case FPE_FLTRES:
                    log(ERROR, "Caught SIGFPE: (floating-point inexact result)\n");
                    break;
                case FPE_FLTINV:
                    log(ERROR, "Caught SIGFPE: (floating-point invalid operation)\n");
                    break;
                case FPE_FLTSUB:
                    log(ERROR, "Caught SIGFPE: (subscript out of range)\n");
                    break;
                default:
                    log(ERROR, "Caught SIGFPE: Arithmetic Exception\n");
                    break;
            }
        case SIGILL:
            switch(siginfo->si_code)
            {
                case ILL_ILLOPC:
                    log(ERROR, "Caught SIGILL: (illegal opcode)\n");
                    break;
                case ILL_ILLOPN:
                    log(ERROR, "Caught SIGILL: (illegal operand)\n");
                    break;
                case ILL_ILLADR:
                    log(ERROR, "Caught SIGILL: (illegal addressing mode)\n");
                    break;
                case ILL_ILLTRP:
                    log(ERROR, "Caught SIGILL: (illegal trap)\n");
                    break;
                case ILL_PRVOPC:
                    log(ERROR, "Caught SIGILL: (privileged opcode)\n");
                    break;
                case ILL_PRVREG:
                    log(ERROR, "Caught SIGILL: (privileged register)\n");
                    break;
                case ILL_COPROC:
                    log(ERROR, "Caught SIGILL: (coprocessor error)\n");
                    break;
                case ILL_BADSTK:
                    log(ERROR, "Caught SIGILL: (internal stack error)\n");
                    break;
                default:
                    log(ERROR, "Caught SIGILL: Illegal Instruction\n");
                    break;
            }
            break;
        case SIGTERM:
            log(ERROR, "Caught SIGTERM: a termination request was sent to the program\n");
            break;
        case SIGABRT:
            log(ERROR, "Caught SIGABRT: usually caused by an abort() or assert()\n");
            break;
        default:
            break;
    }
    posix_print_stack_trace();
    _Exit(1);
}

static uint8_t alternate_stack[SIGSTKSZ];
void set_signal_handler()
{
    /* setup alternate stack */
    {
        stack_t ss = {};
        /* malloc is usually used here, I'm not 100% sure my static allocation
         is valid but it seems to work just fine. */
        ss.ss_sp = (void*)alternate_stack;
        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;

        if (sigaltstack(&ss, NULL) != 0) { err(1, "sigaltstack"); }
    }

    /* register our signal handlers */
    {
        struct sigaction sig_action = {};
        sig_action.sa_sigaction = posix_signal_handler;
        sigemptyset(&sig_action.sa_mask);

#ifdef __APPLE__
        /* for some reason we backtrace() doesn't work on osx
             when we use an alternate stack */
        sig_action.sa_flags = SA_SIGINFO;
#else
        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif

        if (sigaction(SIGSEGV, &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGFPE,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGILL,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGABRT, &sig_action, NULL) != 0) { err(1, "sigaction"); }
    }
}
#endif

int  divide_by_zero();
void cause_segfault();
void stack_overflow();
void infinite_loop();
void illegal_instruction();
void cause_calamity();

void cause_calamity()
{
    /* uncomment one of the following error conditions to cause a calamity of
   your choosing! */

    // (void)divide_by_zero();
    cause_segfault();
    // assert(false);
    // infinite_loop();
    // illegal_instruction();
    // stack_overflow();
}



int divide_by_zero()
{
    int a = 1;
    int b = 0;
  return a / b;
}

void cause_segfault()
{
    int * p = (int*)0x12345678;
    *p = 0;
}

void stack_overflow();
void stack_overflow()
{
    int foo[1000];
    (void)foo;
    stack_overflow();
}

/* break out with ctrl+c to test SIGINT handling */
void infinite_loop()
{
    while(1) {};
}

void illegal_instruction()
{
    /* I couldn't find an easy way to cause this one, so I'm cheating */
    raise(SIGILL);
}

