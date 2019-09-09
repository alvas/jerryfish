/*
 * Line.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef LINE
#define LINE

#include "Point.hpp"

namespace Geom
{
    struct Line
    {
        Point p1, p2;
    };

    struct Line3D
    {
        Point3D p1, p2;
    };
}

#endif
