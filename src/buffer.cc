/*
 * buffer.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "buffer.hpp"
#include "task_strategy.hpp"

using namespace JerryFish;

CacheBuffer::CacheBuffer(int cap, int strategy): Buffer(cap)
{ 
    if (strategy == BufferStrategy::MAX_VALUE)
    {
        m_strategy = new Strategy(this);
        m_useMaxValueStrategy = true;
    }
    else if (strategy == BufferStrategy::MIN_VALUE)
    {
        m_strategy = new Strategy(this);
        m_useMaxValueStrategy = false;
    }
    else if (strategy == BufferStrategy::MAX_NORMALIZED_VALUE)
    {
        m_strategy = new NormalizedValueStrategy(this);
        m_useMaxValueStrategy = true;
    }
    else if (strategy == BufferStrategy::MIN_NORMALIZED_VALUE)
    {
        m_strategy = new NormalizedValueStrategy(this);
        m_useMaxValueStrategy = false;
    }
    else if (strategy == BufferStrategy::MAX_LEFT_TASK_AGE)
    {
        m_strategy = new LeftTaskAgeStrategy(this);
        m_useMaxValueStrategy = true;
    }
    else
    {
        m_strategy = new LeftTaskAgeStrategy(this);
        m_useMaxValueStrategy = false;
    }
}

CacheBuffer::~CacheBuffer()
{
    delete m_strategy;
}

