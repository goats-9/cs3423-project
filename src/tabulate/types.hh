#ifndef TYPES_HH
#define TYPES_HH

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
        int _hour, _min, _sec;
        time();
        time(std::string str);
    };
}

#endif // TYPES_HH