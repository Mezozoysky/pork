#pragma once

#include <SFML/Graphics/RenderWindow.hpp>


namespace sfmk::base
{

class AbstractApplication
{
public:
    AbstractApplication() noexcept;
    virtual ~AbstractApplication() noexcept = default;

    AbstractApplication(AbstractApplication const&) = delete;
    AbstractApplication const& operator=(AbstractApplication const&) = delete;

    int run();
    inline void stop() noexcept
    {
        mIsRunning = false;
        onStop();
    }
    inline bool isRunning() const noexcept
    {
        return mIsRunning;
    }

protected:
    virtual void onInitialize() = 0;
    virtual void onConfigure() = 0;
    virtual void onStart() = 0;
    virtual void onStop() = 0;
    virtual void onStoped() = 0;
    virtual void onUninitialize() = 0;

private:
    void initialize();
    void configure();
    void uninitialize();
    int loop();

private:
    bool mIsInitialized;
    bool mIsRunning;

    sf::RenderWindow mWindow;
};


template<typename ContextT>
class BasicApplication
: public AbstractApplication
{
public:
    using ContextType = ContextT;
public:
    BasicApplication()
    : AbstractApplication()
    {}
    virtual ~BasicApplication() = default;

    inline ContextType * const context() const noexcept
    {
        return &mContext;
    }
private:
    ContextType mContext;
};


} // namespace sfmk::base
