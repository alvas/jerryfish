/*
 * process.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "task.hpp"
#include "task_type.hpp"
#include "buffer.hpp"

namespace JerryFish
{
    /*
     *  Process class contains buffers on which tasks can be placed.
     *  It is a singleton class. It provides API to allow client to
     *  add tasks to and delete tasks from process. 
     */
    class Process
    {
        public:
            /*
             *  GetInstance() is singleton interface to get the instance of process.
             */
            static Process* GetInstance();

            /*
             *  Destroy() is singleton interface to destroy the instance of process.
             */
            static void Destroy();

            /*
             *  Add() adds task to process m_tasks set.
             *  If task is nullptr, it doesn't nothing;
             *  If a specific buffer is available, task is placed on that buffer.
             *  If overflow buffer is available, task is placed on overflow buffer.
             *  If all buffers are full or task is invalid, i.e. task's m_task json is null, 
             *  task is not placed on any buffer, but would
             *  be kept in m_tasks. Delete() method would delete it.
             */
            void Add(Task* task);

            /*
             *  Delete() removes a valid task from m_tasks and  adds it to m_invalidTasks.
             *  When an task is really deleted, it would be removed from m_invalidTasks.
             *  The pupose of having both m_tasks and m_invalidTasks at the same time
             *  is because we don't know the task of timeout and deliver event of a specific
             *  task. If we delete the task immediately, then the later event comes would 
             *  encounter an invalid pointer.
             */
            void Delete(Task* task);

            /*
             *  Deliver() is called when a driver delivers an task. The task would
             *  become invalid. It calls Delete() internally.
             */
            void Deliver(Task* task);

            /*
             *  DeleteInvalidTask() finds out all invalid tasks from m_tasks and m_invalidTasks.
             *  Then call Delete to delete invalid tasks.
             */
            void DeleteInvalidTask();

            /*
             *  Size() returns number of tasks on buffers.
             */
            size_t Size() { return m_tasks.size(); };

            /*
             *  GetCapacity() return capacities of all buffers in process.
             */
            int GetCapacity() { return HOT_BUFFER_CAPACITY + COLD_BUFFER_CAPACITY + FROZEN_BUFFER_CAPACITY + CACHE_BUFFER_CAPACITY; }

        private:
            const int NUM_BUFFER = 4; // total number of buffers in process
            const int CACHE_BUFFER_INDEX = NUM_BUFFER - 1; // index of overflow buffer in process's buffers vector
            const int HOT_BUFFER_CAPACITY = 15;
            const int COLD_BUFFER_CAPACITY = 15;
            const int FROZEN_BUFFER_CAPACITY = 15;
            const int CACHE_BUFFER_CAPACITY = 20;
            const std::vector<int> BUFFER_CAPACITY = {HOT_BUFFER_CAPACITY, COLD_BUFFER_CAPACITY, FROZEN_BUFFER_CAPACITY, CACHE_BUFFER_CAPACITY}; // capacity of buffers

           /*
             *  Process is a singleton class. GetInstance is interface to query instace of Process.
             *  Destroy is interface to destory instance of Process.
             *  Client needs to all Destroy to clean up the instance when the processs shutdown.
             */
            static Process* m_instance;

            /*
             *  m_tasks is used to track all valid tasks created and palce on buffer. 
             *  If there is no space, an task would be dropped and be saved in m_invalidTasks. 
             */
            std::unordered_set<Task*> m_tasks;

            /*
             *  m_invalidTasks saves invalid tasks which can't be placed on buffer. 
             *  There are multiple reasons invalid task is created.
             *  Invalid json is passed to Task's constructor. A new created valid task 
             *  is dropped because there is no spces in buffers.
             */
            std::unordered_set<Task*> m_invalidTasks;

            /*
             *  m_buffers is a vector of buffers referenced by process.
             *  Total number if buffers of process is defined as const number NUM_BUFFER.
             *  Constructor of Process would create buffers.
             *  Destructor of Process would delete buffers.
             */
            std::vector<Buffer*> m_buffers;

            /*
             *  m_taskMutex is used in Add(), Delete(), DeleteInvalidTask() and ShiftCacheTask() 
             *  methods to protect process tasks being adding and deleting at the same time.
             */
            static std::recursive_mutex m_taskMutex;

            Process();

            virtual ~Process();

            /*
             *  IsTaskValid returns true if task is not nullptr and its an m_task json not null;
             *  Otherwise return false.
             */
            bool IsTaskValid(Task* task)
            {
                return task && task->IsValid();
            }

            /*
             *  IsTaskOverflow checks returns true an task is currently placed on overflow buffer.
             *  Otherwise return false.
             */
            bool IsTaskOverflow(Task* task)
            {
                return task && m_tasks.count(task) && m_buffers[CACHE_BUFFER_INDEX]->HasTask(task);
            }

            /*
             *  GetBufferIdx returns index of buffer on which the task is currently placed.
             *  If task is nullptr or is not placed on any buffer, GetBufferIdx would return 0 
             *  by default. 
             */
            int GetBufferIdx(Task* task)
            {
                if (!task || !m_tasks.count(task)) 
                {
                    return 0;
                }
                else 
                {
                    for (int i = 0; i < NUM_BUFFER; ++i)
                    {
                        if (m_buffers[i]->HasTask(task)) 
                        {
                            return i;
                        }
                    }
                }

                return 0;
            }

            /*
             *  GetOriginalBufferIdx would return the index of buffer on which
             *  the task is supposed to be placed.
             *  It can only return index of hight/medium/low buffer. Since an task
             *  should never be placed on overflow buffer originally. But if it does
             *  this would return OVERFLOW_BUFFER_INDEX;
             *  If task is nullptr, it would return 0. Caller of this routine
             *  should make sure task is not nullptr;
             *  This routine doesn't check whether the task has been placed on 
             *  any buffer.
             */
            int GetOriginalBufferIdx(Task* task);

            /*
             *  ShiftCacheTask first removes an task from overflow buffer.
             *  If the task still has value and its original buffer is available, 
             *  place it there; Otherwise destroy the task.
             *  If task is invalid or not-overflow, does nothing.
             */
            void ShiftCacheTask(Task* task);
    };
} // namespace Process
