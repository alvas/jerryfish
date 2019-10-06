/*
 * Debugger.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "debugger.hpp"

#pragma once

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <execinfo.h>
#include <iostream>

#include <boost/stacktrace.hpp>

void printTrace()
{
    void* callstack[2048];
    int frames = backtrace(callstack, 2048);
    char** strs = backtrace_symbols(callstack, frames);

    for (int i = 0; i < frames; ++i) {
        std::cout << strs[i] << std::endl;
    }

    free(strs);
}

void printTraceBoost()
{
    std::cout << boost::stacktrace::stacktrace();
}

#endif /* !DEBUGGER_H */
