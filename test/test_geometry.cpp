/*
 * test_geometry.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "test_geometry.h"

#include <iostream>
#include <fstream>
#include <random>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;


int main(int argc, char** argv)
{
    
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> distribution(-500.0, 500.0);

    std::ofstream myfile;
    myfile.open("points.txt");

    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            myfile << distribution(generator);

            if (j < 2) myfile << ',';
        }

        myfile << std::endl;
    }

    myfile.close();

    std::ifstream data("points.txt", std::ifstream::binary);
    std::ofstream jsonFile;
    jsonFile.open("points.json");
    jsonFile << "{\"points\": [";

    if (data.is_open())
    {
        std::string line;
        bool isInitial = true;

        while (getline(data, line))
        {
            if (isInitial)
            {
                isInitial = false;
            }
            else
            {
                jsonFile << ",";
            }

            jsonFile << "[" << line << "]";
        }
    }

    jsonFile << "]}";
    jsonFile.close();

    std::ostringstream ss;
    ss << jsonFile.rdbuf();
    std::string json = ss.str();
    Document d;
    d.Parse(json.c_str());
    Value& a = d["points"];

    for (SizeType i = 0; i < a.Size(); i++)
        std::cout << a[i].GetDouble() << std::endl;

    return 0;
}
