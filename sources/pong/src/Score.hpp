#pragma once


namespace pong
{

struct Score
{
    float x;
    float y;
    int value;

    Score()
    : x{0.f}
    , y{0.f}
    , value{0}
    {}
};

} // namespace pong
