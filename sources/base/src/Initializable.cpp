#include "Initializable.hpp"

namespace sfmk::base
{

Initializable::Initializable() noexcept
: mIsInitialized{false}
{}

void Initializable::initialize()
{
    if (!mIsInitialized)
    {
        onInitialize();
        mIsInitialized = true;
    }
}

void Initializable::uninitialize()
{
    if (mIsInitialized)
    {
        mIsInitialized = false;
        onUninitialize();
    }
}

} // namespace sfmk::base
