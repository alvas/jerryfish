/*
 * RandomPointsGenerator.hxx
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <fstream>
#include <random>
#include <sstream>
#include <vector>

#include "rapidjson/document.h"
#include "geometry/Point.hpp"

using namespace rapidjson;
using namespace Geom;

namespace Geom
{
    template <class T>
    class PointGenerator
    {
    public:
        PointGenerator(const std::string& file): m_file(file){}

        void CreatePoints()
        {
            std::random_device rd;  //Will be used to obtain a seed for the random number engine
            std::mt19937 generator(rd()); //Standard mersenne_twister_engine seeded with rd()
            std::uniform_real_distribution<> distribution(m_l, m_r);

            // generate points set
            std::ofstream myfile;
            std::string fileName(m_file);
            fileName += ".txt";
            myfile.open(fileName.c_str());

            for (int i = 0; i < m_num; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    myfile << distribution(generator);

                    if (j < 1) myfile << ',';
                }

                myfile << std::endl;
            }

            myfile.close();
        }

        void CreatePointsJson()
        {
            CreatePoints();

            std::string fileName(m_file), fileName2(m_file);
            fileName += ".txt";
            fileName2 += ".json";
            std::ifstream data(fileName.c_str(), std::ifstream::binary);
            std::ofstream jsonFile;
            jsonFile.open(fileName2.c_str());
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
        }

        std::vector<T> ReadPoints()
        {
            std::string fileName(m_file);
            fileName += ".json";
            std::ifstream ss(fileName.c_str());
            std::stringstream buffer;
            buffer << ss.rdbuf();

            Document d;
            d.Parse(buffer.str().c_str());
            Value& a = d["points"];
            std::vector<Point> result;

            for (auto &v: a.GetArray())
            {
                std::vector<double> p;

                for (auto &b: v.GetArray())
                    p.push_back(b.GetDouble());

                result.push_back({p[0], p[1]});
            }

            return result;
        }

        void SetNum(int num) { m_num = num; }
        void SetRange(double l, double r) { m_l = l; m_r = r; }

    private:
        std::string m_file;
        double m_l = -500, m_r = 500;
        int m_num = 10;
    };
}


