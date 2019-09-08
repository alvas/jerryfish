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
        double x, y;

        bool operator<(const Point& p) const
        {
            return (x < p.x) || (x == p.x && y < p.y);
        }
    };

    struct Point3D
    {
        double x, y, z;

        bool operator<(const Point3D& p) const
        {
            return (x < p.x) || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z);
        }
    };
};

#endif
