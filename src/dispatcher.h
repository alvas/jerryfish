/*
 * dispatcher.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */


#pragma once

#include <thread>

#include "process.hpp"

namespace JerryFish
{
    class Dispatcher
    {
        public:
            static void Add(Task* task)
            {
                std::thread dispatcher(dispatch, task);
                dispatcher.detach();
            }
        private:
            static void dispatch(Task* task)
            {
                Process* process = Process::GetInstance();
                process->Add(task);
            }
    };
} // namespace JerryFish
