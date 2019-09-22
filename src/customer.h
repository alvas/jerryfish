/*
 * customer.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

namespace JerryFish
{
    class Customer
    {
        public:
            const int PORT = 8080;

            void operator()(std::vector<std::string> *tasks)
            {
                if (!tasks) return;

                int sock = 0; 

                if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
                { 
                    std::cout << "\n Socket creation error" << std::endl;; 
                    exit(EXIT_FAILURE); 
                } 

                struct sockaddr_in serv_addr; 
                serv_addr.sin_family = AF_INET; 
                serv_addr.sin_port = htons(PORT); 

                if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)  
                { 
                    std::cout << "\nInvalid address/ Address not supported" << std::endl; 
                    exit(EXIT_FAILURE); 
                } 

                if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
                { 
                    std::cout << "\nConnection Failed" << std::endl; 
                    exit(EXIT_FAILURE); 
                } 

                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator (seed);
                std::poisson_distribution<int> distribution(3.25);    
                srand(time(nullptr));

                for (auto o: *tasks)
                {
                    int sec = distribution(generator);
                    std::this_thread::sleep_for(std::chrono::seconds(sec));
                    std::cout << "waiting for " << sec << " seconds to send task " << o << std::endl;
                    send(sock , o.data(), o.size(), 0); 
                }

                close(sock); 
            }
    };
} // namespace JerryFish
