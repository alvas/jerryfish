/*
 * Circle.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef CIRCLE
#define CIRCLE

#include "Point.hpp"

namespace Geom
{
    struct Circle
    {
        Circle(): c(), r(0) {}
        Circle(const Point& p, double r1): c(p), r(r1) {}
        Circle(double x, double y, double r1): c(x, y), r(r1) {}
        Point c;
        double r;
    };

    struct Sphere
    {
        Sphere(): c(), r(0) {}
        Sphere(const Point3D& p, double r1): c(p), r(r1) {}
        Sphere(double x, double y, double z, double r1): c(x, y, z), r(r1) {}
        Point3D c;
        double r;
    };
}

#endif
