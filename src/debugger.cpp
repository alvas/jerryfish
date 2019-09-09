/*
 * debugger.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "debugger.hpp"
//
#include <execinfo.h>

#include <iostream>

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
