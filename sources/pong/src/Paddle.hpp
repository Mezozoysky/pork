#pragma once


namespace pong
{

struct Field;

struct Paddle
{
    float x;
    float y;
    float width;
    float height;

    Paddle()
    : x{0.f}
    , y{0.f}
    , width{0.f}
    , height{0.f}
    {}

    void setY(Field const & field, float value);
};
} // namespace pong
