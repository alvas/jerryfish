/*
 * ConvexHull.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef MONOTONE_CHAIN
#define MONOTONE_CHAIN

#include <algorithm>
#include <vector>
#include "Point.hpp"
#include "Calculation.hpp"

using namespace std;
using namespace Geom;

// https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
//
// Implementation of Andrew's monotone chain 2D convex hull algorithm.
// Asymptotic complexity: O(n log n).
// Practical performance: 0.5-1.0 seconds for n=1000000 on a 1GHz machine.

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
vector<Point> ConvexHull(vector<Point>& P)
{
    size_t n = P.size(), k = 0;

    if (n <= 3) return P;

    vector<Point> H(2*n);

    // Sort points lexicographically
    sort(P.begin(), P.end());

    // Build lower hull
    for (size_t i = 0; i < n; ++i) 
    {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;

        H[k++] = P[i];
    }

    // Build upper hull
    for (size_t i = n-1, t = k+1; i > 0; --i) {

        while (k >= t && cross(H[k-2], H[k-1], P[i-1]) <= 0) k--;

        H[k++] = P[i-1];
    }

    H.resize(k-1);
    return H;
}

#endif
