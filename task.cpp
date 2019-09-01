/*
 * task.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <boost/algorithm/string.hpp>

#include "task.hpp"

using namespace JerryFish;

std::atomic<unsigned int> JerryFish::Task::m_idSeed(0);

Task::Task(json11::Json& o) 
{
    m_id = ++m_idSeed;
    m_name = o["name"].string_value();
    m_category = o["category"].string_value();
    m_bufferLife = o["life"].int_value();
    m_dropRate = o["dropRate"].number_value();
    m_createdTime = time(nullptr);
    m_cacheTime = m_createdTime;
    m_hasValue = true;
    m_factor = 1;

    boost::algorithm::to_lower(m_category);

    // Check task content and make this task invalid if necessary.
    if (m_name.empty() ||
        !IsValidTemp(m_category) ||
        m_bufferLife <= 0 ||
        m_dropRate <= 0)
    {
        m_hasValue = false;
    }
}

double Task::GetValue()
{
    if (!m_hasValue || m_bufferLife <= 0) return 0;

    double age = GetAge();
    double cacheAge = GetCacheAge();
    double dropRate = m_dropRate;
    int factor = -m_factor + 2;

    return (m_bufferLife - age) - 
        (dropRate * factor * (age - cacheAge)) -
        (dropRate * 2 * cacheAge);
}
