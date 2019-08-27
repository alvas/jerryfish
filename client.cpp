/*
 * customer.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#include "json11/json11.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <random>

const int PORT = 8080;

using namespace std;
using namespace json11;

string fileName2("./test/test1.json");

void readTasks(vector<string>& tasks)
{
    string file;
    std::ifstream data(fileName2, std::ifstream::binary);

    if (data.is_open())
    {
        string line;

        while (getline(data, line))
        {
            file += line;
        }

        data.close();
    }

    
    auto bItr = file.find('{');
    string err;

    while (bItr != string::npos) {
        auto eItr = file.find('}', bItr);

        if (eItr != string::npos) {
            auto str = file.substr(bItr, eItr - bItr + 1);
            tasks.push_back(str);
        }

        bItr = file.find('{', eItr);
    }
}


int main(int argc, char** argv)
{
    int sock = 0; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        cout << "\n Socket creation error \n"; 
        exit(EXIT_FAILURE); 
    } 
   
    struct sockaddr_in serv_addr; 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)  
    { 
        cout << "\nInvalid address/ Address not supported \n"; 
        exit(EXIT_FAILURE); 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout << "\nConnection Failed \n"; 
        exit(EXIT_FAILURE); 
    } 

    vector<string> tasks;
    readTasks(tasks);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::poisson_distribution<int> distribution(3.25);    
    srand(time(nullptr));

    for (auto o: tasks)
    {
        int sec = distribution(generator);
        std::this_thread::sleep_for(std::chrono::seconds(sec));
        cout << o << endl;
        send(sock , o.data(), o.size(), 0); 
    }

    close(sock); 

    return 0;  
}
