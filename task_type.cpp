/*
 * task_type.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "task_type.hpp"

using namespace JerryFish;

static std::unordered_map<std::string, int> bufferIdxMap = {
    {"hight", 0},
    {"medium", 1},
    {"low", 2}
};

int JerryFish::GetOriginalBufferIdx(const std::string& category)
{
    return bufferIdxMap[category];
}

bool JerryFish::IsValidTemp(const std::string& category)
{
    return bufferIdxMap.count(category) != 0;
}
