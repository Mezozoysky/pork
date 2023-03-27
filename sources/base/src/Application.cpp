#include "Application.hpp"
#include "Exception.hpp"
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace sfmk::base
{

AbstractApplication::AbstractApplication() noexcept
: mIsInitialized{false}
, mIsRunning{false}
{}

int AbstractApplication::run()
{
    int exitCode{1};
    try
    {
        initialize();
        configure();
        mIsRunning = true;
        exitCode = loop();
        uninitialize();
    }
    catch(Exception & e)
    {
        // log.error(e.what());
        return e.code();
    }
    catch(std::exception & e)
    {
        // log.error(e.what())
        return 1; // unhandled runtime error
    }
    catch(...)
    {
        // log.error("Unhandled unknown exception")
        return 1;
    }
    return exitCode;
}

void AbstractApplication::initialize()
{
    if (mIsInitialized)
    {
        return;
    }

    spdlog::set_default_logger(spdlog::stdout_color_mt(""));
    spdlog::default_logger()->set_level(spdlog::level::info);
    spdlog::info("Start logging");
    spdlog::warn("some warning");
    spdlog::error("some error");
    spdlog::critical("some critical");

    mWindow.create(sf::VideoMode(1024, 768), "sfmk");

    onInitialize();
    mIsInitialized = true;
}

void AbstractApplication::uninitialize()
{
    if (!mIsInitialized)
    {
        return;
    }

    mIsInitialized = false;

    onUninitialize();

    if (mWindow.isOpen())
    {
        mWindow.close();
    }
}

void AbstractApplication::configure()
{
    onConfigure();
}

int AbstractApplication::loop()
{
    onStart();

    while (mIsRunning && mWindow.isOpen())
    {
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    mWindow.close();
                    break;
                case sf::Event::LostFocus:
                    break;
                case sf::Event::GainedFocus:
                    break;
                case sf::Event::TextEntered:
                    break;
                case sf::Event::KeyPressed:
                    break;
                case sf::Event::KeyReleased:
                    break;
                case sf::Event::MouseWheelMoved:
                    break;
                case sf::Event::MouseWheelScrolled:
                    break;
                case sf::Event::MouseButtonPressed:
                    break;
                case sf::Event::MouseButtonReleased:
                    break;
                case sf::Event::MouseMoved:
                    break;
                case sf::Event::JoystickButtonPressed:
                    break;
                case sf::Event::JoystickButtonReleased:
                    break;
                case sf::Event::JoystickMoved:
                    break;
                case sf::Event::JoystickConnected:
                    break;
                case sf::Event::JoystickDisconnected:
                    break;
                case sf::Event::TouchBegan:
                    break;
                case sf::Event::TouchMoved:
                    break;
                case sf::Event::TouchEnded:
                    break;
                case sf::Event::SensorChanged:
                    break;
                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
            {
                mWindow.close();
            }
        }
        if (mWindow.isOpen())
        {
            mWindow.clear();
            // draw
            mWindow.display();
        }
    }

    onStoped();
    return 0;
}

} // namespace sfmk::base
