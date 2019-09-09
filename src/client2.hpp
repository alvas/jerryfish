/*
 * client2.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "app.hpp"

namespace JerryFish
{
    class Client: public App
    {
        public:
            Client(int num = 20): m_numCustomer(num), m_taskNum(50), m_auto(false), m_minLife(0), m_maxLife(300), m_minRate(0), m_maxRate(0.7) {}
            
            /*
             *  Run() creates multiple child processes in task to send data to server.
             */
            void Run();

            /*
             *  SetCustomerNum() allows client to specify using how many processes to send
             *  data to server.
             */
            void SetCustomerNum(int num) { m_numCustomer = num; }

            void SetAutoGenerateTask(bool createTask) { m_auto = createTask; }

            void SetBufferLife(int minLife = 0, int maxLife = 300) { m_minLife = minLife;  m_maxLife = maxLife; }

            void SetRate(double minRate = 0, double maxRate = 0.7) { m_minRate = minRate; m_maxRate = maxRate; }

            void SetTaskNumber(int num) { m_taskNum = num; }

            const std::vector<std::string> GetTasks() { return m_tasks; }

            void ClearTasks() { m_tasks.clear(); }
        private:
            /*
             *  RunCustomer() is called by child process to open connect and send data.
             */
            void RunCustomer();

            int m_numCustomer;

            int m_taskNum;

            bool m_auto;

            int m_minLife, m_maxLife;

            double m_minRate, m_maxRate;
    };
}
