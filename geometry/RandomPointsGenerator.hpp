/*
 * RandomPointsGenerator.hxx
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef RANDOM_POINT_GENERATOR
#define RANDOM_POINT_GENERATOR

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
        PointGenerator(){}
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
                for (int j = 0; j < T::DIMENSION; ++j)
                {
                    myfile << distribution(generator);

                    if (j < T::DIMENSION - 1) myfile << ',';
                }

                myfile << std::endl;
            }

            myfile.close();
        }

        std::vector<T> CreatePoints1()
        {
            std::random_device rd;  //Will be used to obtain a seed for the random number engine
            std::mt19937 generator(rd()); //Standard mersenne_twister_engine seeded with rd()
            std::uniform_real_distribution<> distribution(m_l, m_r);

            // generate points set
            std::vector<T> result;

            for (int i = 0; i < m_num; ++i)
            {
                std::vector<double> t;

                for (int j = 0; j < T::DIMENSION; ++j)
                {
                    t.push_back(distribution(generator));
                }

                result.push_back(t);
            }

            return result;
        }

        void CreatePointsJson()
        {
            CreatePoints();

            std::string fileName(m_file);
            std::ifstream data(std::string(fileName + ".txt").c_str(), std::ifstream::binary);
            std::ofstream jsonFile;
            jsonFile.open(std::string(fileName + ".json").c_str());
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
            std::vector<T> result;

            for (auto &v: a.GetArray())
            {
                std::vector<double> t;

                for (auto &b: v.GetArray())
                    t.push_back(b.GetDouble());

                result.push_back(t);
            }

            return result;
        }

        std::vector<T> ReadPoints(const std::string& file)
        {
            std::ifstream ss(file.c_str());
            std::stringstream buffer;
            buffer << ss.rdbuf();

            Document d;
            d.Parse(buffer.str().c_str());
            Value& a = d["points"];
            std::vector<T> result;

            for (auto &v: a.GetArray())
            {
                std::vector<double> t;

                for (auto &b: v.GetArray())
                    t.push_back(b.GetDouble());

                result.push_back(t);
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

#endif
