/*
 * task_cleaner.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <thread>
#include <iostream>

#include "process.hpp"

namespace JerryFish
{
    /*
     *  TaskCleaner class is a functor which is called by a timer to delete invalid task
     *  perriodically.
     *  When an invalid json string is used to create Task, task's json part is null.
     *  If the server allows to add invalid task to process, a timer would be set to clean
     *  them up.
     */
    class TaskTimer
    {
        public:
            void operator()(int expire)
            {
                JerryFish::Process* process = JerryFish::Process::GetInstance();

                while (true)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(expire));
                    process->DeleteInvalidTask();
                }
            }
    };
} // namespace JerryFish

