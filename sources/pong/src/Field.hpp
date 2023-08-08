#pragma once


namespace pong
{

struct Field
{
    float x;
    float y;
    float width;
    float height;

    Field()
    : x{0.f}
    , y{0.f}
    , width{0.f}
    , height{0.f}
    {}
};

} // namespace pong
