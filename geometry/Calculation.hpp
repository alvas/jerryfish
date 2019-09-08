/*
 * Calculation.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef CALCULATION
#define CALCULATION

#include "Point.hpp"

double cross(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

Point3D cross(const Point3D &A, const Point3D &B)
{
    return {A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x};
}

Point center(const Point& A, const Point& B)
{
    return {(A.x + B.x) / 2, (A.y + B.y) / 2};
}

Point center(const Point& A, const Point& B, const Point& C)
{
    double a = A.x - B.x;
    double b = A.y - B.y;
    double c = A.x - C.x;
    double d = A.y - C.y;
    double e = ((A.x * A.x - B.x * B.x) + (A.y * A.y - B.y * B.y)) / 2;
    double f = ((A.x * A.x - C.x * C.x) + (A.y * A.y - C.y * C.y)) / 2;
    double det = b * c - a * d;

    if (fabs(det) < 1e5)
    {
        return Point();
    }

    double x0 = -(d * e - b * f) / det;
    double y0 = -(a * f - c * e) / det;
    return Point(x0, y0);
}

Point3D center(const Point3D &A, const Point3D &B)
{
    return {(A.x + B.x) / 2, (A.y + B.y) / 2, (A.z + B.z) / 2};
}

double distance(const Point& A, const Point& B)
{
    double x = fabs(A.x - B.x), y = fabs(A.y - B.y);
    return sqrt(x * x + y * y);
}

double distance(const Point3D &A, const Point3D &B)
{
    double x = fabs(A.x - B.x), y = fabs(A.y - B.y), z = fabs(A.z - B.z);
    return sqrt(x * x + y * y + z * z);
}

#endif
