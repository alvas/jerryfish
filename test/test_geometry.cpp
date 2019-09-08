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

#include "geometry/RandomPointsGenerator.hpp"
#include "geometry/MonotoneChain.hpp"
#include "geometry/Welzl.hpp"

using namespace rapidjson;


int main(int argc, char** argv)
{
    PointGenerator<Point> pg("points");
    pg.SetNum(3);
    //pg.CreatePointsJson();
    std::vector<Point> &&points = pg.CreatePoints1();

    auto hull = ConvexHull(points);

    std::cout << "convex hull is: " << std::endl;

    for (auto p: hull)
    {
        std::cout << p.x << "\t" << p.y << std::endl;
    }

    auto c = Welzl(points);

    std::cout << "circle center: " << c.c.x << ", " << c.c.y << "; radius: " << c.r << std::endl; 
    return 0;
}
