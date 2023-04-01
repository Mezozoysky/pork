#pragma once

#include <stdexcept>


namespace pork::core
{

class Exception
: public std::runtime_error
{
public:
    Exception(char const * message, int code = 1);

    inline int code()
    {
        return mCode;
    }

private:
    int mCode;
};


} // namespace pork::core
