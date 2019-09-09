/*
 * client2_app.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "client2_app.h"

#include "client2.hpp"

#include <string>

using namespace JerryFish;

std::string fileName;
std::string fileName2("./test/test1.json");

int main(int argc, char** argv)
{
    int num  = 2;

    if (argc > 1 && atoi(argv[1]) > 0) 
    {
        num = atoi(argv[1]);
    }

    Client client(num);
    client.ReadTasks(fileName);
    client.Run();

    return 0;  
}
