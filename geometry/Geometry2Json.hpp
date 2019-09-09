/*
 * Geometry2Json.hxx
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
            std::string result("{[");
            result += to_string(p.x) + "," + to_string(p.y);
            result += "]}";
            return result;
        }

        static std::string Parse(const std::vector<Point>& ps)
        {
            std::string result("{\"points\":[");
            bool init = true;

            for (auto& p: ps)
            {
                if (init) {
                    init = false;
                }
                else
                {
                    result += ",";
                }

                result += "[" + to_string(p.x) + "," + to_string(p.y) + "]";
            }

            result += "]}";
            return result;
        }
    };
}

#endif
