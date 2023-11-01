#pragma once

#include <string>
#include <vector>

namespace tabulate {

    std::vector<std::string> split(std::string s, std::string del);
    
    class date {
    public:
        int _year, _month, _day;
        date();
        date(std::string str);
    };

    class time {
    public:
        int _hour, _min, _sec;
        time();
        time(std::string str);
    };
}
