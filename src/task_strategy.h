/*
 * task_strategy.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <queue>
#include <utility>

#include "buffer.hpp"
#include "process.hpp"

namespace JerryFish
{
    /*  
     *  Strategy class implement stategy design pattern. It decides what tasks to choose 
     *  from cache buffer to shift to a specific buffer.
     *
     *  GetTaskValue() is an interface subclass can override to return value in strategy
     *  to choose task.
     */
    class Strategy
    {
        public:
            Strategy(CacheBuffer* buffer): m_buffer(buffer) { }
            virtual ~Strategy() {}
            
            template <bool T>
            std::vector<Task*> GetTasks(int index, int num)
            {
                // categorylate boolean parameter T specifies categorylate function using max/min value priority queue
                typedef typename std::conditional< T, std::less<std::pair<double, Task*>>, std::greater<std::pair<double, Task*>> >::type ComparatorType;

                std::priority_queue<std::pair<double, Task*>, std::vector<std::pair<double, Task*>>, ComparatorType> pq;

                for (auto o: m_buffer->m_tasks)
                {
                    auto task = m_buffer->m_idToTaskMap[o];

                    // Only return tasks that are supposed to shift to the buffer with index
                    if (GetOriginalBufferIdx(task->GetTemp()) != index) continue;

                    double value = GetTaskValue(task);
                    pq.push({value, task});
                }

                std::vector<Task*> tasks;

                // pq keeps the highest/lowest normalized value task at top.
                // Only return the first num tasks.
                for (int i = 0; i < num && !pq.empty(); ++i)
                {
                    tasks.push_back(pq.top().second);
                    pq.pop();
                }

                return tasks;
            }

        protected:
            /*
             *  GetTaskValue() is an interface that allows subclass can override to use
             *  in different stragety.
             */
            virtual double GetTaskValue(Task* task)
            {
                return task->GetValue();
            }

        private:
            CacheBuffer* m_buffer;
    };

    class NormalizedValueStrategy: public Strategy
    {
        public:
            NormalizedValueStrategy(CacheBuffer* buffer): Strategy(buffer) {}
            
        protected:
            virtual double GetTaskValue(Task* task)
            {
                // use normalzied value strategy
                int bufferLife = task->GetBufferLife();
                if (bufferLife <= 0) return 0;

                return task->GetValue() / task->GetBufferLife();
            }
    };

    class LeftTaskAgeStrategy: public Strategy
    {
        public:
            LeftTaskAgeStrategy(CacheBuffer* buffer): Strategy(buffer) {}
        protected:
            virtual double GetTaskValue(Task* task)
            {
                // value formula of cache buffer task:
                // value = (BufferLife - TaskAge) - (2 * DropRate * TaskAge)
                // When value becomes 0:
                // 0 = (BufferLife - TaskAge) - (2 * DropRate * TaskAge);
                // let x be the portion of TaskAge that has passed;
                // let y be the portion of TaskAge that is left;
                // 0 = (BufferLife - (x + y)) - (2 * DropRate * (x + y));
                // y = (BufferLife) / (1 + 2 * DropRate) - x;
                return task->GetBufferLife() / (1 + 2 * task->GetDropRate()) - task->GetAge();
            }
    };

} // namespace JerryFish
