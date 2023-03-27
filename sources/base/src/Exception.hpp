#pragma once

#include <exception>
#include <stdexcept>


namespace pork::base
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


} // namespace pork::base
