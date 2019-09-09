/*
 * server.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "server.h"


#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <iostream>
#include <random>
#include <string>

#include "json11/json11.hpp"

#include "driver.hpp"
#include "process.hpp"

using namespace json11;

const int PORT = 8080;
const int BUFSIZE = 1024;

int main(int argc, char const *argv[]) 
{ 
    int sock = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_len = sizeof(sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(0);
    sin.sin_addr.s_addr= INADDR_ANY;
    socklen_t len = sizeof(sin);
       
    if (getsockname(sock, (struct sockaddr *)&sin, &len) < 0)
    { 
        std::cerr << "\n Socket creation error \n"; 
        exit(EXIT_FAILURE); 
    } 

    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
       
    if (bind(sock, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        std::cerr << "bind failed\n"; 
        exit(EXIT_FAILURE); 
    } 

    if (listen(sock, 3) < 0) 
    { 
        std::cerr << "listen\n"; 
        exit(EXIT_FAILURE); 
    } 

    int new_socket = 0;

    if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
    { 
        std::cerr << "accept\n"; 
        exit(EXIT_FAILURE); 
    } 

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(2, 10);
    srand(time(nullptr));

    JerryFish::Process* process = JerryFish::Process::GetInstance();

    std::string err;

    while (true)
    {
        char buffer[BUFSIZE] = {0}; 
        int numBuf = read(new_socket, buffer, BUFSIZE);

        if (!numBuf) break;

        std::string str(buffer, numBuf);
        auto json = json11::Json::parse(str, err);

        JerryFish::Task *task = new JerryFish::Task(json);
        process->Add(task);
        std::thread driver(JerryFish::Driver(), task, distribution(generator));
        driver.detach();

        //std::cout << json.dump() << std::endl;
        task->dump();
    }

    JerryFish::Process::Destroy();

    return 0; 
} 
