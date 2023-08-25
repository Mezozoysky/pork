#include <pork/base/Context.hpp>
#include <pork/base/Application.hpp>
#include <string_view>
#include <SDL3/SDL.h>
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Field.hpp"
#include "Score.hpp"

using namespace std::literals;

namespace pong
{

class App : public pork::base::Application
{
public:
    App();
    virtual ~App();

    int onConfigure(std::vector<std::string_view> const & args,
                    pugi::xml_document const & configXml) override;
    int onSetUp() override;
    int onStart() override;
    void onShutDown() override;
    void iterate() override;

private:
    void handleInput();
    void updateBall();
    void updateCollisions();
    void updateAI();
    void render();

    void onMouseMoved(int x, int y);
    void collideBallWithField();
    bool isCollidingBallWithPaddle(Paddle const & paddle);
    float calcPaddleReflection(Paddle const & paddle, float hitY);
    void collideBallWithPaddle();

private:
    Ball mBall;
    Paddle mPaddleLeft;
    Paddle mPaddleRight;
    Field mField;
    Score mScoreLeft;
    Score mScoreRight;

    SDL_Window * mWindow;
    SDL_Renderer * mRenderer;
};

} // namespace pong
