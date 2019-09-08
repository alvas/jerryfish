/*
 * test_geometry.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "test_geometry.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "geometry/RandomPointsGenerator.hpp"
#include "geometry/MonotoneChain.hpp"

using namespace rapidjson;


int main(int argc, char** argv)
{
    PointGenerator<Point> pg("points");
    pg.SetNum(10000);
    pg.CreatePointsJson();
    std::vector<Point> &&points = pg.ReadPoints();

    auto hull = ConvexHull(points);

    std::cout << "convex hull is: " << std::endl;

    for (auto p: hull)
    {
        std::cout << p.x << "\t" << p.y << std::endl;
    }

    return 0;
}
