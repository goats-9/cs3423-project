#pragma once

#include <string>
#include <vector>
#include <iostream>
namespace yy
{
    std::string &operator<<(std::string &s, const std::string &c);
    std::string &operator<<(std::string &s, const char *c);
}

namespace tabulate
{

    std::vector<std::string> split(std::string s, std::string del);

    class date
    {
    public:
        int _year, _month, _day;
        date();
        date(std::string str);
    };

    class time
    {
    public:
        int _hour, _min, _sec;
        time();
        time(std::string str);
    };

    class constant
    {
    public:
        std::string value;
        std::string type;

        constant();
        constant(std::string _type, std::string _value);
    };
}
