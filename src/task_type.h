/*
 * task_type.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <string>
#include <unordered_map>

namespace JerryFish
{
    /*
     *  GetOriginalBufferIdx() takes a string of category and returns
     *  the index of buffer on which it is supposed to be placed originally.
     *  This is an API which is used in Process and Buffer classes.
     *  Off course, we can save the buffer index value inside the Task class.
     *  And query index would be faster than comparing string and return
     *  the index from a map here.
     *  But we have to move this mapping to Task class. That would make
     *  the Task class to have buffer mapping information.
     */
    int GetOriginalBufferIdx(const std::string& category);

    /*
     *  IsValidTemp() checks whether category is a valid task buffer categoryerature:
     *  hight/medium/low.
     */
    bool IsValidTemp(const std::string& category);
} // namespace JerryFish
