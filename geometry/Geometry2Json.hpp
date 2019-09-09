/*
 * Geometry2Json.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef GEOMETRY_TO_JSON
#define GEOMETRY_TO_JSON

#include "rapidjson/document.h"

namespace Geom
{
    class Geom2Json
    {
    public:
        static std::string Parse(const Circle& c)
        {
            std::string result;
            return result;
        }

        static std::string Parse(const Point& p)
        {
            std::stringstream buffer;
            buffer << std::setprecision(7) << "{[" << p.x << "," << p.y << "]}";
            return buffer.str();
        }

        static std::string Parse(const std::vector<Point>& ps)
        {
            std::stringstream buffer;
            buffer << std::setprecision(7) << "{\"points\":["; 
            bool init = true;

            for (auto& p: ps)
            {
                if (init) {
                    init = false;
                }
                else
                {
                    buffer << ",";
                }

                buffer << "[" << p.x << "," << p.y << "]";
            }

            buffer << "]}";

            return buffer.str();
        }
    };
}

#endif
