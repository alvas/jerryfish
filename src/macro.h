/*
 * macro.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MACRO_H
#define MACRO_H

#include <string.h>
#include <assert.h>
#include "Logger.h"
//#include "Utils.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define JERRYFISH_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define JERRYFISH_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define JERRYFISH_LIKELY(x)      (x)
#   define JERRYFISH_UNLIKELY(x)      (x)
#endif

/// 断言宏封装
#define JERRYFISH_ASSERT(x) \
    if(JERRYFISH_UNLIKELY(!(x))) { \
        JERRYFISH_LOG_ERROR(JERRYFISH_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << JerryFish::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

/// 断言宏封装
#define JERRYFISH_ASSERT2(x, w) \
    if(JERRYFISH_UNLIKELY(!(x))) { \
        JERRYFISH_LOG_ERROR(JERRYFISH_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << JerryFish::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif /* !MACRO_H */
