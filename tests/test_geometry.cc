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
#include <cmath>

#include "gtest/gtest.h"

#include "geometry/RandomPointsGenerator.hpp"
#include "geometry/ConvexHull.hpp"
#include "geometry/SmallestCircle.hpp"
#include "geometry/Tolerance.hpp"
#include "geometry/Geometry2Json.hpp"

using namespace rapidjson;
using namespace Geom;

TEST(GEOMTEST, SMALLESTCIRCLE)
{
    PointGenerator<Point> pg;
    //pg.SetNum(3);
    //pg.CreatePointsJson();
    //std::vector<Point> &&points = pg.CreatePoints1();
    std::vector<Point> &&points = pg.ReadPoints("tests/points1.json");

    auto hull = ConvexHull(points);

    auto c = Welzl(points);

    std::cout << "circle center: " << c.c.x << ", " << c.c.y << "; radius: " << c.r << std::endl; 

    EXPECT_TRUE(fabs(c.c.x - 0.0) < TOL && fabs(c.c.y - 0.0) < TOL);
}

TEST(GEOMTEST, GEOM2JSON)
{
    PointGenerator<Point> pg;
    std::vector<Point> &&points = pg.ReadPoints("tests/points1.json");

    ASSERT_EQ(points.size(), 3);
    EXPECT_STREQ(Geom2Json::Parse(points[0]).c_str(), std::string("{[1,0]}").c_str());
}

TEST(GEOMTEST, CALC)
{
    PointGenerator<Point> pg;
    std::vector<Point> &&points = pg.ReadPoints("tests/points1.json");

    ASSERT_EQ(points.size(), 3);
    auto c = center(points[0], points[1], points[2]);
    EXPECT_STREQ(Geom2Json::Parse(c).c_str(), std::string("{[0,0]}").c_str());
}
