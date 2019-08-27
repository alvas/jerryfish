/*
 * task.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <atomic>
#include <iostream>

#include "json11/json11.hpp"

#include "task_type.hpp"

namespace JerryFish
{
    /*
     *  Task class saves task information.
     */
    class Task
    {
        public:
            Task(json11::Json& o);

            int GetId()
            {
                return m_id;
            }

            const std::string& GetName()
            {
                return m_name;
            }

            const std::string& GetTemp()
            {
                return m_category;
            }

            int GetBufferLife()
            {
                return m_bufferLife;
            }

            double GetDropRate()
            {
                return m_dropRate;
            }

            time_t GetCreatedTime()
            {
                return m_createdTime;
            }

            double GetAge()
            {
                return difftime(time(nullptr), m_createdTime);
            }

            /*
             *  GetCacheAge returns the time an task has been placed
             *  on an cache buffer.
             */
            double GetCacheAge()
            {
                return difftime(m_cacheTime, m_createdTime);
            }
            
            double GetValue();

            void AddToCacheBuffer()
            {
                m_factor = 2;
            }

            void SetCacheTime()
            {
                m_factor = 1;
                m_cacheTime = time(nullptr);
            }

            bool HasValue()
            {
                if (m_hasValue)
                {
                    m_hasValue = GetValue() > 0;
                }

                return m_hasValue;
            }

            void Drop()
            {
                m_hasValue = false;
            }

            /*
             *  IshValid returns true if task's json is not null
             *  and it has value.
             */
            bool IsValid()
            {
                return !m_name.empty() && !m_category.empty() && HasValue();
            }

            /*
             *  Return normalized value of an task.
             */
            double GetNormalizedValue()
            {
                if (!m_hasValue) return 0;

                // Invalid task may have 0 bufferLife which has 0 value.
                // return 0 and void divide by 0 calculation.
                if (m_bufferLife <= 0) return 0;

                return GetValue() / m_bufferLife;
            }

            void dump()
            {
                std::cout << m_id << "\t" << m_createdTime << "\t" <<  m_name << "\t"
                <<  m_category << "\t" << m_bufferLife << "\t"
                << m_dropRate << "\t" << GetValue() << "\t"
                << GetNormalizedValue() << std::endl;
            }

        private:
            /*
             *  m_idSeed is used to generate id for task. Atomic makes sure
             *  it is thread safe for increment.
             */
            static std::atomic<unsigned int> m_idSeed;

            /*
             *  m_task is a json object which saves detail of the task
             *
             *  name, string
             *  category, string
             *  life, int
             *  dropRate, double
             */
            //json11::Json m_task;

            /*
             *  m_id is unique id of an task.
             */
            int m_id;

            /*
             *  m_name is task's name.
             */
            std::string m_name;

            /*
             *  m_category is task's buffer categoryerature.
             */
            std::string m_category;

            /*
             *  m_bufferLife is task's buffer life.
             */
            int m_bufferLife;

            /*
             *  m_dropRate is task's buffer drop rate.
             */
            double m_dropRate;

            /*
             *  m_createdTime saves the time when the task was created.
             */
            time_t m_createdTime;

            /*
             *  Period how long task is placed on cache buffer.
             *
             *  The initial value would be the same as m_createdAt.
             *  If an task was never placed on cache buffer, difference
             *  between m_cacheTime and m_createdTime would be 0.
             *
             *  If an task was place on cache buffer, when it is removed
             *  from cache buffer, the current time would be set to this
             *  attribute. The difference between m_cacheTime and m_createdTime
             *  is the time period of cache.
             */
            time_t m_cacheTime;

            /*
             *  m_factor is a factor used in drop formula.
             *  Its initial value is 1. If an task is placed on its 
             *  specified buffer, this value is always 1. If an task
             *  is placed on cache buffer, AddToCacheBuffer sets
             *  it to 2. When an task is removed from cache buffer,
             *  SetCacheTime would set it back to 1.
             *
             *  value drop formula:
             *
             *  f(x) = -x + 2;
             *  
             *  x = 1, f(x) = 1;
             *  x = 2, f(x) = 0;
             *
             *  value = (bufferLife - age)
             *          - (dropRate * f(x) * (age - cacheAge))
             *          - (dropRate * 2 * cacheAge);
             *
             *  When an task is placed at the specific buffer,
             *  its cacheAge is 0. The 3rd part of the formula
             *  is 0; The 2nd part of the formula is dropRate * age.
             *
             *  When an task is placed at the cache buffer at the 
             *  begining, the 2nd part of the formular is 0. drop rate
             *  is double. After the task is moved from cache buffer
             *  to the specific buffer, the formular is
             *  (bufferLife - age) minus the value dayed on the cache buffer 
             *  (dropRate * 2 * cacheAge) and minus the value drops 
             *  on the specific buffer (dropRate * (age - cacheAge)).
             */
            int m_factor;

            /*
             *  m_hasValue indicates whether the task still has value.
             *  Its initial value is set to true. Every time HasValue
             *  method is called, it would cache the result. Once the
             *  task doesn't have any value, it would return immediately
             *  to avoid unnecessary computation in GetValue.
             */
            bool m_hasValue;
    };
} // namespace Process
