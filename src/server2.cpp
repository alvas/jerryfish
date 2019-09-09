/*
 * server2.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "server2.hpp"


#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "driver.hpp"
#include "dispatcher.hpp"
#include "json11/json11.hpp"
#include "process.hpp"
#include "task_timer.hpp"
#include "server2.hpp"

using namespace JerryFish;

void Server::Run()
{   
    int master_socket = 0;

    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        std::cerr << "socket failed" << std::endl;   
        exit(EXIT_FAILURE);   
    }   
     
    int opt = 1;   
    //set master socket to allow multiple connections
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)   
    {   
        std::cerr << "setsockopt" << std::endl;   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    struct sockaddr_in address;   
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);   
    }   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    {   
        std::cerr << "listen" << std::endl;   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    int addrlen = sizeof(address);   

    //set of socket descriptors  
    fd_set readfds;   
    std::set<int> client_socket;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(2, 10);
    srand(time(nullptr));

    //JerryFish::Process* kicken = JerryFish::Process::GetInstance();

    std::vector<std::thread> threads;
    std::string err;

    std::thread cleanUpInvalidTask(JerryFish::TaskTimer(), 3);
    cleanUpInvalidTask.detach();
    std::unordered_map<int, std::string> socketBuffer;

    while (true)   
    {   
        // clear the socket set
        FD_ZERO(&readfds);   
     
        // add master socket to set  
        FD_SET(master_socket, &readfds);   
        int max_sd = master_socket;   
             
        // add child sockets to set  
        for (auto client: client_socket)   
        {   
            if(client > 0) FD_SET(client , &readfds);   
            if(client > max_sd) max_sd = client;   
        }   
     
        // wait for an activity on one of the sockets , timeout is NULL, so wait indefinitely  
        if (select(max_sd + 1 , &readfds , NULL , NULL , NULL) < 0)   
        {   
            std::cerr << "select error" << std::endl;   
        }   
             
        // if something happened on the master socket, then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
            int new_socket = 0;

            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)   
            {   
                std::cerr << "accept" << std::endl;   
                exit(EXIT_FAILURE);   
            }   
             
            client_socket.insert(new_socket);
            socketBuffer[new_socket].resize(1024);
        }   

        std::set<int> oldSocket(client_socket.begin(), client_socket.end());

        // its some IO operation on some other socket 
        for (auto client: oldSocket)   
        {   
            if (FD_ISSET(client, &readfds))   
            {   
                int numBuff = 0;

                // check if it was for closing, and also read the incoming message  
                if ((numBuff = read(client, (void*)socketBuffer[client].c_str(), 1024)) == 0)   
                {   
                    close(client);
                    client_socket.erase(client);
                    socketBuffer.erase(client);
                }   
                else 
                {
                    std::string str(socketBuffer[client].begin(), socketBuffer[client].begin() + numBuff), str2;

                    json11::Json json;
                    size_t bItr = 0, eItr = str.find("}{");
                    JerryFish::Task *task = nullptr;

                    while (eItr != std::string::npos)
                    {
                        str2 = str.substr(bItr, eItr - bItr + 1);
                        json = json11::Json::parse(str2.c_str(), err);

                        task = new JerryFish::Task(json);
                        JerryFish::Dispatcher::Add(task);
                        threads.push_back(std::thread(JerryFish::Driver(), task, distribution(generator)));
                        bItr = eItr + 2;
                        eItr = str.find("}{", bItr);
                    }

                    str2 = str.substr(bItr);
                    json = json11::Json::parse(str.c_str(), err);

                    task = new JerryFish::Task(json);
                    JerryFish::Dispatcher::Add(task);

                    threads.push_back(std::thread(JerryFish::Driver(), task, distribution(generator)));
                }
            }
        }

        if (!m_shutdown && client_socket.empty()) break;
    }   

    std::for_each(threads.begin(), threads.end(), [](std::thread& t){ t.join(); });

    JerryFish::Process::Destroy();
}   
