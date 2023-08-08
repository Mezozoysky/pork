#include "App.hpp"
#include <pork/core/Logging.hpp>
#include <SDL_video.h>


namespace pong
{

App::App()
: pork::base::Application("PorkDemo"sv, "PorkDynamics"sv)
{}

App::~App()
{}

int App::onConfigure(std::vector<std::string_view> const & args, pugi::xml_document const & configXml)
{
    return 0;
}

int App::onSetUp()
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    mWindow = SDL_CreateWindow("PORK PONG",
                               800, 600, SDL_WINDOW_INPUT_FOCUS);

    if (!mWindow)
    {
        return -1;
    }

    mRenderer = SDL_CreateRenderer(mWindow, 0, SDL_RENDERER_ACCELERATED);

    if (!mRenderer)
    {
        return -1;
    }

    return 0;
}

int App::onStart()
{
    return 0;
}

void App::onShutDown()
{
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
    }
    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void App::iterate()
{
    handleInput();
    updateBall();
    updateCollisions();
    updateAI();
    render();
}

void App::handleInput()
{
}

void App::updateBall()
{
}

void App::updateCollisions()
{
}

void App::updateAI()
{
}

void App::render()
{
}

void App::onMouseMoved(int x, int y)
{
    mPaddleLeft.setY(mField, y - mPaddleLeft.height / 2.f);
}

void App::collideBallWithField()
{
    if (mBall.getY() < mField.y)
    {
        mBall.setPosition(mBall.getX(), mField.y);
        mBall.setDirection(mBall.getDirX(), -mBall.getDirY());
    }
    else if (mBall.getY() + mBall.getHeight() > mField.y + mField.height)
    {
        mBall.setPosition(mBall.getX(), mField.y + mField.height - mBall.getHeight());
        mBall.setDirection(mBall.getDirX(), -mBall.getDirY());
    }

    if (mBall.getX() <= mField.x)
    {
        mBall.setPosition(mPaddleRight.x - 2.f * mBall.getWidth(),
                          mField.y + mField.height / 2.f - mBall.getHeight() / 2.f);
        mBall.setDirection(-1.f, -1.f);
        mScoreRight.value += 1;
    }
    else if (mBall.getX() + mBall.getWidth() >= mField.x + mField.width)
    {
        mBall.setPosition(mPaddleLeft.x + mPaddleLeft.width + mBall.getWidth(),
                          mField.y + mField.height / 2.f - mBall.getHeight() / 2.f);
        mBall.setDirection(-1.f, 1.f);
    }
}

bool App::isCollidingBallWithPaddle(Paddle const & paddle)
{
    if (mBall.getX() + mBall.getWidth() > paddle.x
        and mBall.getX() < paddle.x + paddle.width
        and mBall.getY() + mBall.getHeight() > paddle.y
        and mBall.getY() < paddle.y + paddle.height)
    {
        return true;
    }
    return false;
}

float App::calcPaddleReflection(const Paddle &paddle, float hitY)
{
    if (hitY < 0.f)
    {
        hitY = 0.f;
    }
    else if (hitY > paddle.height)
    {
        hitY = paddle.height;
    }

    hitY -= paddle.height / 2.f;

    return 2.f * (hitY / (paddle.height / 2.f));
}

void App::collideBallWithPaddle()
{
    float ballCenterY = mBall.getY() + mBall.getHeight() / 2.f;

    if (isCollidingBallWithPaddle(mPaddleLeft))
    {
        mBall.setPosition(mPaddleLeft.x + mPaddleLeft.width,
                          mBall.getY());
        mBall.setDirection(1, calcPaddleReflection(mPaddleLeft, ballCenterY - mPaddleLeft.y));
    }

    if (isCollidingBallWithPaddle(mPaddleRight))
    {
        mBall.setPosition(mPaddleRight.x - mBall.getWidth(),
                          mBall.getY());
        mBall.setDirection(-1, calcPaddleReflection(mPaddleRight, ballCenterY - mPaddleRight.y));
    }
}

} // namespace pong

