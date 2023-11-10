#pragma once

#include <string>
#include <vector>

namespace yy
{
    std::string &operator<< (std::string&s,std::string a);
    std::string &operator<< (std::string&s,const char* a);
}
namespace tabulate {

    // std::vector<std::string> split(std::string s, std::string del);
    
    // class date {
    // public:
    //     int _year, _month, _day;
    //     date();
    //     date(std::string str);
    // };

    // class time {
    // public:
    //     int _hour, _min, _sec;
    //     time();
    //     time(std::string str);
    // };
    typedef struct vector_of_string
    {
        std::vector<std::string> sem;
        std::string trans;
    } vector_of_string;
    typedef struct vector_of_int
    {
        std::vector<int> sem;
        std::string trans;
    } vector_of_int;
    typedef struct Int
    {
        int sem;
        std::string trans;
    }Int;
    typedef struct String
    {
        std::string sem;
        std::string trans;
    } String;
    typedef std::string Default;
    typedef struct constant
    {
        std::string type;
        std::string value;
    } constant;
}
