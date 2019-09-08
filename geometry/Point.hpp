/*
 * Point.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef POINT
#define POINT

namespace Geom
{
    struct Point
    {
        static const int DIMENSION = 2;

        double x, y;

        Point(): x(0), y(0) {}
        Point(double x1, double y1): x(x1), y(y1){}
        Point(const std::vector<double> c): x(c[0]), y(c[1]) {}
        Point(double c[2]): x(c[0]), y(c[1]) {}

        bool operator<(const Point& p) const
        {
            return (x < p.x) || (x == p.x && y < p.y);
        }
    };

    struct Point3D
    {
        static const int DIMENSION = 3;

        double x, y, z;

        Point3D(): x(0), y(0), z(0) {}
        Point3D(double x1, double y1, double z1): x(x1), y(y1), z(z1) {}
        Point3D(const std::vector<double> c): x(c[0]), y(c[1]), z(c[2]) {}
        Point3D(double c[3]): x(c[0]), y(c[1]), z(c[2]) {}

        bool operator<(const Point3D& p) const
        {
            return (x < p.x) || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z);
        }
    };
};

#endif
