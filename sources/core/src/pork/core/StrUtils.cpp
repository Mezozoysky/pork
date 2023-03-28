#include "StrUtils.hpp"
#include <algorithm>
#include <sstream>
#include <cstring>


using namespace std::literals;

namespace pork::core
{

constexpr char const * WHITESPACE{" \n\t\v\r\f"};


bool isWhitespace(unsigned char c)
{
    bool found{false};
    for (std::size_t index = 0u; index < std::strlen(WHITESPACE); ++index)
    {
        if (c == WHITESPACE[index])
        {
            found = true;
            break;
        }
    }
    return (found);
}

void trimLeft(std::string & text)
{
    std::size_t first = text.find_first_not_of(WHITESPACE);
    if (first == std::string::npos)
    {
        text.clear();
        return;
    }
    text = text.substr(first);
}

void trimRight(std::string &text)
{
    std::size_t last = text.find_last_not_of(WHITESPACE);
    if (last == std::string::npos)
    {
        text.clear();
        return;
    }
    text = text.substr(0u, last);
}


void trim(std::string & text)
{
    std::size_t first = text.find_first_not_of(WHITESPACE);
    if (first == std::string::npos)
    {
        text.clear();
        return;
    }
    std::size_t last = text.find_last_not_of(WHITESPACE);
    text = text.substr(first, last - first + 1u);
}

std::string join(const StrList &strList, std::string_view delim)
{
    std::ostringstream oss;

    if (!strList.empty())
    {
        oss << strList[0u];
    }
    for (std::size_t index = 1u; index < strList.size(); ++index)
    {
        oss << delim << strList[index];
    }

    return oss.str();
}

void toUpperIn(std::string & text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
    {
        return std::toupper(c);
    });
}

std::string toUpper(std::string_view text)
{
    std::string upper{text};
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c)
    {
        return std::toupper(c);
    });
    return upper;
}
void toLowerIn(std::string & text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
    {
        return std::tolower(c);
    });
}
std::string toLower(std::string_view text)
{
    std::string lower{text};
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c)
    {
        return std::tolower(c);
    });
    return lower;
}

bool parseBool(std::string const & str, bool & value, std::string_view t, std::string_view f)
{
    if (str.empty())
    {
        return false;
    }

    std::string lowerStr = toLower(str);
    std::string lowerT = toLower(t);
    std::string lowerF = toLower(f);
    if (lowerStr == lowerT)
    {
        value = true;
        return true;
    }
    else if (lowerStr == lowerF)
    {
        value = false;
        return true;
    }

    return false;
}

} // namspace pork::core
