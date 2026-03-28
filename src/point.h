#ifndef POINT_H
#define POINT_H

#include <Arduino.h>

constexpr uint16_t width = 127;
constexpr uint16_t height = 63;

struct Point
{
    uint16_t x = 0;
    uint16_t y = 0;

    constexpr Point(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}

    Point withOffset(uint16_t _x, uint16_t _y = 0) const
    {
        return Point(x + _x, y + _y);
    }
    Point() = default;
};

namespace DefaultPoints
{
    constexpr Point LeftTop{0, 0};
    constexpr Point RightTop{width, 0};
    constexpr Point LeftBottom{0, height};
    constexpr Point RightBottom{width, height};
    constexpr Point CenterDown{width / 2, height};
    constexpr Point Center{width / 2, height / 2};
    constexpr Point CenterLeft{0, height / 2};
    constexpr Point CenterRight{width, height / 2};
}

#endif