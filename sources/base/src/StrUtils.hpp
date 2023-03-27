#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <limits>


namespace pork::base
{

using StrList = std::vector<std::string>;
using SVList = std::vector<std::string_view>;

bool isWhitespace(unsigned char c);

void trimLeft(std::string & text);
void trimRight(std::string & text);
void trim(std::string & text);

std::string join(StrList const & strList,
                 std::string_view delim = std::string_view_literals::operator""sv("\n", 1));
//     StrList split(std::string_view text, std::string_view delim);

void toUpperIn(std::string & text);
std::string toUpper(std::string_view text);
void toLowerIn(std::string & text);
std::string toLower(std::string_view text);

bool parseBool(std::string const & str, bool & value,
               std::string_view t = std::string_view_literals::operator""sv("true", 4),
               std::string_view f = std::string_view_literals::operator""sv("false", 5));

template<typename T>
bool parseNumber(std::string const & str, T & value)
{
    static_assert(std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value, "T isn't number");
    if (str.empty())
    {
        return false;
    }
    bool isFloat{std::is_floating_point<T>::value};
    bool isSigned{std::is_signed<T>::value};
    try
    {
        if (isFloat)
        {
            long double wide = std::stold(str);
            if (wide >= std::numeric_limits<T>::min() && wide <= std::numeric_limits<T>::max())
            {
                value = static_cast<T>(wide);
                return true;
            }
        }
        else if (isSigned)
        {
            long long wide = std::stoll(str);
            if (wide >= std::numeric_limits<T>::min() && wide <= std::numeric_limits<T>::max())
            {
                value = static_cast<T>(wide);
                return true;
            }
        }
        else
        {
            unsigned long long wide = std::stoull(str);
            if (wide <= std::numeric_limits<T>::max())
            {
                value = static_cast<T>(wide);
                return true;
            }
        }
    }
    catch (std::exception const & e)
    {
    }

    return false;
}

} // namspace pork::base
