/*
 * app.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

const int PORT = 8080;

namespace JerryFish
{
    class App
    {
        public:
            void ReadTasks(std::string fileName)
            {
                std::string file;
                std::ifstream data(fileName, std::ifstream::binary);

                if (data.is_open())
                {
                    std::string line;

                    while (getline(data, line))
                    {
                        file += line;
                    }

                    data.close();
                }


                auto bItr = file.find('{');
                std::string err;

                while (bItr != std::string::npos) {
                    auto eItr = file.find('}', bItr);

                    if (eItr != std::string::npos) {
                        auto str = file.substr(bItr, eItr - bItr + 1);
                        if (!str.empty()) m_tasks.push_back(str);
                    }

                    bItr = file.find('{', eItr);
                }
            }

        protected:
            std::vector<std::string> m_tasks;
    };

}
