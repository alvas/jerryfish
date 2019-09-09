/*
 * SmallestCircle.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef WELZL
#define WELZL

#include "Circle.hpp"

using namespace Geom;

// https://en.wikipedia.org/wiki/Smallest-circle_problem

Circle trivial(vector<Point>& P)
{
    if (P.empty())
    {
        return Circle();
    }
    else if (P.size() == 1)
    {
        return Circle(P[0], 0);
    }
    else if (P.size() == 2)
    {
        return Circle(center(P[0], P[1]), distance(P[0], P[1]) / 2);
    }
    else if (P.size() == 3)
    {
        auto c = center(P[0], P[1], P[2]);
        return Circle(c, distance(P[0], c) / 2);
    }
    else
    {
        return Circle();
    }
}

Circle Welzl(vector<Point>& P, vector<Point>& R)
{
    if (P.size() < 3)
    {
        return trivial(P);
    }
    else if (R.size() == 3)
    {
        return trivial(R);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, P.size());
    int idx = distribution(generator);
    auto p = P[idx];
    swap(P[idx], P.back());
    P.resize(P.size() - 1);
    auto D = Welzl(P, R);

    if (distance(D.c, p) < D.r)
    {
        return D;
    }
    else
    {
        R.push_back(p);
        return Welzl(P, R);
    }
}

Circle Welzl(vector<Point>& P)
{
    vector<Point> R;
    return Welzl(P, R);
}

#endif
