/*
 * buffer.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <queue>
#include <utility>
#include <unordered_map>
#include <unordered_set>

#include "task.hpp"

namespace JerryFish
{
    /*
     *  Buffer class saves tasks have been placed on it.
     */
    class Buffer
    {
        public:
            Buffer(int cap): m_tasks(), m_capacity(cap) {}

            /*
             *  When a buffer is destroyed all tasks on the buffer are destroyed.
             */
            virtual ~Buffer() { }

            /*
             *  Add() places an task on the buffer, if task is valid and buffer is not full.
             */
            virtual void Add(Task* task) 
            {
                if (!task || !task->IsValid() || IsFull()) return;
                m_tasks.insert(task->GetId());
            }

            /*
             *  Remove() removes task from a buffer. It doesn't destroy the task.
             */
            virtual void Remove(Task* task)
            {
                m_tasks.erase(task->GetId());
            }

            /*
             *  Return true if the buffer is full.
             */
            virtual bool IsFull()
            {
                return m_tasks.size() >= m_capacity;
            }

            /*
             *  Return true if task is not nullptr and on the buffer
             */
            virtual bool HasTask(Task* task)
            {
                return task && m_tasks.count(task->GetId());
            }

            /*
             *  Return size of tasks on the buffer.
             */
            size_t Size()
            {
                return m_tasks.size();
            }

            /*
             *  Return free space of a buffer.
             */
            virtual int AvailableCacheSize()
            {
                return m_capacity - m_tasks.size();
            }

            /*
             *  Return an empty list. 
             *  Subclass can override this method to return a list
             *  of tasks which are ready to shift.
             *  Input:
             *  index - index of buffer on which tasks are going to shift to.
             *  num - number of tasks going to shift.
             */
            virtual std::vector<Task*> GetCacheTasks(int index, int num)
            {
                return {};
            }

            virtual void PrintTasks() {}
        protected:
            /*
             *  m_tasks references all tasks have been placed on the buffer.
             */
            std::unordered_set<unsigned int> m_tasks;

        private:
            /*
             *  Capacity of a buffer.
             */
            size_t m_capacity;
    };

    class Strategy;

    enum BufferStrategy
    {
        MAX_VALUE, // max value strategy
        MIN_VALUE, // min value strategy
        MAX_NORMALIZED_VALUE, // max normalized strategy
        MIN_NORMALIZED_VALUE, // min normalized strategy
        MAX_LEFT_TASK_AGE, // max left task age
        MIN_LEFT_TASK_AGE, // min left task age
    };

    /*
     *  CacheBuffer saves tasks placed on overflow buffer.
     */
    class CacheBuffer: public Buffer
    {
        friend class Strategy;

        public:
            CacheBuffer(int cap, int strategy);

            ~CacheBuffer();

            virtual void Add(Task* task) 
            {
                Buffer::Add(task);
                m_idToTaskMap[task->GetId()] = task;
            }

            /*
             *  When an task is removed from an overflow buffer, 
             *  it removes the task from m_tasks set.
             *  Cache buffer would set overflow time to the task.
             *  (Cache time - Create time) is the age that
             *  an task's drop rate is two times of the original drop
             *  rate.
             *  By setting this task's GetCacheAge and GetValue
             *  could calculate the task value regarding the overflow
             *  drop rate.
             */
            virtual void Remove(Task* task)
            {
                Buffer::Remove(task);
                m_idToTaskMap[task->GetId()]->SetCacheTime();
                m_idToTaskMap.erase(task->GetId());
            }

            /*
             *  Return 0 since we don't allow to shift tasks
             *  from other buffer to overflow buffer.
             */
            virtual int AvailableCacheSize()
            {
                return 0;
            }

            /*
             *  Return a list of tasks which are ready to shift 
             *  from overflow buffer to other buffer. 
             *  Input:
             *  index - index of buffer on which tasks are going to shift to.
             *  num - number of tasks going to shift.
             */
            virtual std::vector<Task*> GetCacheTasks(int idx, int num);

            virtual void PrintTasks()
            {
                for (auto o: m_tasks)
                {
                    auto task = this->m_idToTaskMap[o];
                    task->dump();
                }
            }
        private:
            /*
             *  m_idToTaskMap saves the task id to task pointer mapping information.
             *  The mapping is aded in Add() method. And is removed by Remove().
             *  Using a pointer allows us to invoke SetCacheTime() on the task
             *  when it is removed form overflow buffer.
             */
            std::unordered_map<unsigned int, Task*> m_idToTaskMap;

            /*
             *  m_strategy points to strategy object. It allows clients
             *  to choose a way to shift task out of overflow buffer
             *  from a family of strategies.
             */
            Strategy* m_strategy;

            /*
             *  m_useMaxValueStrategy tells strategy object to use max or min value
             *  when picking task to shift out of overflow buffer.
             */
            bool m_useMaxValueStrategy;
    };
} // namespace JerryFish
