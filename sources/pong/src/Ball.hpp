#pragma once


namespace pong
{

struct Field;

struct Ball
{

    Ball();

    inline float getX() const { return mX; }
    inline float getY() const { return mY; }
    inline float getWidth() const { return mWidth; }
    inline float getHeight() const { return mHeight; }
    inline float getDirX() const { return mDirX; }
    inline float getDirY() const { return mDirY; }

    void setPosition(float x, float y);
    void setDirection(float dirX, float dirY);
    void setSpeed(float speed);
    void updateVelocity();

    void Step(float deltaTime);

private:
    float mX;
    float mY;
    float mWidth;
    float mHeight;
    float mDirX;
    float mDirY;
    float mVelocityX;
    float mVelocityY;
    float mSpeed;
};

} // namespace pong
