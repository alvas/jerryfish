/*
 * CacheBuffer.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <queue>
#include <utility>

#include "buffer.hpp"
#include "task_strategy.hpp"

using namespace JerryFish;

std::vector<Task*> CacheBuffer::GetCacheTasks(int index, int num)
{
    if (m_useMaxValueStrategy)
    {
        return m_strategy->GetTasks<true>(index, num);
    }
    else
    {
        return m_strategy->GetTasks<false>(index, num);
    }
}

