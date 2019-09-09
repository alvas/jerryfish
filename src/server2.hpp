/*
 * server2.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "app.hpp"

namespace JerryFish
{
    class Server: public App
    {
        public:
            Server(): m_shutdown(false) {}
            /*
             *  Run() listenses to multiple sockets and waiting for connection from client.
             *  Server creates a child thread per task to add task to process.
             */
            void Run();

            void Shutdown() { m_shutdown = true; }
        private:
            bool m_shutdown;
    };
}
