#pragma once

namespace sfmk::base
{

class Initializable
{
public:
    Initializable() noexcept;
    virtual ~Initializable() noexcept = default;

    void initialize();
    void uninitialize();

    inline bool isInitialized() const noexcept
    {
        return mIsInitialized;
    }

    virtual void onInitialize() = 0;
    virtual void onUninitialize() = 0;

protected:
    void setInitialized(bool initialized = true) noexcept;

private:
    bool mIsInitialized;
};


template<typename DerivedT>
class InitializableMixin
: public Initializable
{
public:
    InitializableMixin() noexcept = default;
    virtual ~InitializableMixin() noexcept = default;
};

} // namespace sfmk::base
