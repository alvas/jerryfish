/*
 * process.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <algorithm>

#include "process.hpp"
#include "task_type.hpp"
#include "buffer.hpp"

JerryFish::Process* JerryFish::Process::m_instance = nullptr;
std::recursive_mutex JerryFish::Process::m_taskMutex;

using namespace JerryFish;

Process::Process() 
{
    m_buffers.resize(NUM_BUFFER);

    for (int i = 0; i < NUM_BUFFER; ++i)
    {
        if (i != CACHE_BUFFER_INDEX)
        {
            m_buffers[i] = new Buffer(BUFFER_CAPACITY[i]);
        }
        else
        {
            m_buffers[CACHE_BUFFER_INDEX] = new CacheBuffer(BUFFER_CAPACITY[CACHE_BUFFER_INDEX], BufferStrategy::MIN_LEFT_TASK_AGE);
        }
    }
}

Process::~Process()
{
    std::for_each(m_tasks.begin(), m_tasks.end(), [](Task* o){ delete o; });
    std::for_each(m_invalidTasks.begin(), m_invalidTasks.end(), [](Task* o){ delete o; });
}


Process* Process::GetInstance()
{
    if (!m_instance) 
    {
        m_instance = new Process();
    }

    return m_instance;
}

void Process::Destroy()
{
    if (m_instance) 
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

void Process::Add(Task* task)
{
    if (!task) return;

    std::lock_guard<std::recursive_mutex> lock(m_taskMutex);

    if (IsTaskValid(task))
    {
        int idx = GetOriginalBufferIdx(task);

        // Try to place the task to the buffer it supposed to be first.
        // If the specific buffer is full, try overflow buffer.
        // If no space is available, drop the task and make it invalid.
        if (!m_buffers[idx]->IsFull()) 
        {
            m_buffers[idx]->Add(task);
        }
        else if (!m_buffers[CACHE_BUFFER_INDEX]->IsFull())
        {
            task->AddToCacheBuffer();
            m_buffers[CACHE_BUFFER_INDEX]->Add(task);
        }
        else
        {
            task->Drop();
        }
    }

    // Only task has been place on buffer is valid, add it to m_tasks.
    // Otherwise, save it to m_invalidTasks and delete it later.
    if (IsTaskValid(task)) 
    {
        m_tasks.insert(task);
    }
    else
    {
        m_invalidTasks.insert(task);
    }
}

void Process::Delete(Task* task)
{
    if (!task) return;

    std::lock_guard<std::recursive_mutex> lock(m_taskMutex);

    if (m_tasks.count(task))
    {
        // When an task on buffer is deleted, remove it from buffer.
        // Then drop its value to 0 and move it from m_tasks to m_invalidTasks.
        // Tasks in m_invalidTasks would be really deleted later.
        auto idx = GetBufferIdx(task);
        m_buffers[idx]->Remove(task);
        m_tasks.erase(task);
        task->Drop();
        m_invalidTasks.insert(task);

        // Whenever an task is removed from hight/medium/low buffer,
        // try to move tasks from overflow buffer to it.
        int num = m_buffers[idx]->AvailableCacheSize();
        auto tasks = m_buffers[CACHE_BUFFER_INDEX]->GetCacheTasks(idx, num);
        std::for_each(tasks.begin(), tasks.end(), [this](Task* o){ 
            ShiftCacheTask(o); 
        });
    }
    else if (m_invalidTasks.count(task))
    {
        m_invalidTasks.erase(task);
        delete task;
    }
    else
    {
        // This is left empty intentially. When an invalid task(whose json part is null), it would 
        // be added to m_invalidTasks direclty. Timeout and deliver events would trigger two
        // deletion. The first time it is found in m_invalidTasks and deleted. The second time
        // it would come here.
    }
}

void Process::Deliver(Task* task)
{
    Delete(task);
}

void Process::DeleteInvalidTask()
{
    // Delete() would modify m_tasks and m_invalidTasks, so we can't
    // do it when we loop through these sets. Use another set to collect
    // invalid tasks and call Delete() on them at the end.
    std::unordered_set<Task*> invalidTasks;

    std::lock_guard<std::recursive_mutex> lock(m_taskMutex);

    for (auto task: m_tasks) 
    {
        if (!IsTaskValid(task))
        {
            invalidTasks.insert(task);
        }
    }

    invalidTasks.insert(m_invalidTasks.begin(), m_invalidTasks.end());

    std::for_each(invalidTasks.begin(), invalidTasks.end(), [this](Task* o){ Delete(o); });
}

void Process::ShiftCacheTask(Task* task)
{
    if (!task) return;

    std::lock_guard<std::recursive_mutex> lock(m_taskMutex);

    // Move an task out of overflow buffer and to the buffer it supposed to be placed.
    m_buffers[CACHE_BUFFER_INDEX]->Remove(task);
    m_buffers[GetOriginalBufferIdx(task)]->Add(task);
}

int Process::GetOriginalBufferIdx(Task* task)
{
    if (!task) return 0;
    return JerryFish::GetOriginalBufferIdx(task->GetTemp());
}

