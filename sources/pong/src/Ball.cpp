#include "Ball.hpp"
#include <cmath>
#include "Field.hpp"


namespace pong
{

Ball::Ball()
: mX{0.f}
, mY{0.f}
, mWidth{0.f}
, mHeight{0.f}
, mDirX{0.f}
, mDirY{0.f}
, mSpeed{0.f}
{
    updateVelocity();
}

void Ball::setPosition(float x, float y)
{
    mX = x;
    mY = y;
}

void Ball::setDirection(float dirX, float dirY)
{
    mDirX = dirX;
    mDirY = dirY;
    updateVelocity();
}

void Ball::setSpeed(float speed)
{
    mSpeed = speed;
    updateVelocity();
}

void Ball::updateVelocity()
{
    float velLen = std::sqrt(mDirX * mDirX + mDirY * mDirY);
    mVelocityX = mSpeed * mDirX / velLen;
    mVelocityY = mSpeed * mDirY / velLen;
}

void Ball::Step(float deltaTime)
{
    mX += mVelocityX * deltaTime;
    mY += mVelocityY * deltaTime;
}

} // namespace pong
