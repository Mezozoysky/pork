#include "Paddle.hpp"
#include "Field.hpp"


namespace pong
{

void Paddle::setY(const Field &field, float value)
{
    if (value < field.y)
    {
        value = field.y;
    }
    else if (value + height > field.y + field.height)
    {
        value = field.y + field.height - height;
    }
    else
    {
        // do nothing
    }

    y = value;
}

} // namespace pong
