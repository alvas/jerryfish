/*
 * driver.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <iostream>
#include <thread>

#include "process.hpp"

namespace JerryFish
{
    class Driver
    {
        public:
            void operator()(Task* task, int delay)
            {
                std::this_thread::sleep_for(std::chrono::seconds(delay));
                Process* process = Process::GetInstance();
                process->Deliver(task);
            }
    };
} // namespace JerryFish
