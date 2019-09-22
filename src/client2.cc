/*
 * client2.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */


#include <arpa/inet.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <unistd.h> 

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>

#include "app.hpp"
#include "client2.hpp"

using namespace std;
using namespace JerryFish;

std::string randomString(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        " 0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void Client::RunCustomer()
{
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
    std::default_random_engine generator(seed);
    std::poisson_distribution<int> distribution(3.25);    
    std::uniform_int_distribution<int> tmpDist(0, 2), lifeDist(-m_minLife, m_maxLife);
    std::uniform_real_distribution<double> rateDist(-m_minRate, m_maxRate);
    std::unordered_map<int, std::string> categoryMap = { {0, "hight"}, {1, "medium"}, {2, "low"} };

    srand(time(nullptr));

    if (m_auto)
    {
        for (int i = 0; i < m_taskNum; ++i)
        {
            int sec = distribution(generator);
            std::this_thread::sleep_for(std::chrono::seconds(sec));
            string name = randomString(8);
            int category = tmpDist(generator), life = lifeDist(generator);
            double rate = rateDist(generator);
            std::string o = "{\"name\": \"" + name + "\", \"category\": \"" + categoryMap[category] + "\", \"life\": " + std::to_string(life) + ", \"dropRate\": " + to_string(rate) + "}";
            std::cout << o << std::endl;
            send(sock , o.data(), o.size(), 0); 
        }
    }
    else
    {
        for (auto o: m_tasks)
        {
            int sec = distribution(generator);
            std::this_thread::sleep_for(std::chrono::seconds(sec));
            std::cout << o << std::endl;
            send(sock , o.data(), o.size(), 0); 
        }
    }

    close(sock); 
    exit(0);
}

void Client::Run()
{
    for (int i = 0; i < m_numCustomer; ++i)
    {
        pid_t pid = fork();

        if(pid < 0) /* error */
        {
            std::cerr << "Fork error" << std::endl;
            exit(1); 
        }
        else if (pid == 0) /* child */
        {
            RunCustomer();
        }
        else /* parrent */
        {
        }
    }
}
