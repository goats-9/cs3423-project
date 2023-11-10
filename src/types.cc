#include "../include/types.hh"

// std::vector<std::string> tabulate::split(std::string s, std::string del)
// {
//     std::vector<std::string> res;
//     int start, end = -1 * del.size();
//     do
//     {
//         start = end + del.size();
//         end = s.find(del, start);
//         res.push_back(s.substr(start, end - start));
//     } while (end != -1);

//     return res;
// }

// tabulate::date::date() : _year(0), _month(0), _day(0) 
// {}
// tabulate::date::date(std::string str)
// {
//     std::vector<std::string> temp = tabulate::split(str, "-");
//     _year = stoi(temp[0]);
//     _month = stoi(temp[1]);
//     _day = stoi(temp[2]);
// }

// tabulate::time::time() : _hour(0), _min(0), _sec(0)
// {}
// tabulate::time::time(std::string str)
// {
//     std::vector<std::string> temp = tabulate::split(str, ":");
//     _hour = stoi(temp[0]);
//     _min = stoi(temp[1]);
//     _sec = stoi(temp[2]);
// }

namespace yy
{
    std::string &operator<< (std::string&s,std::string a)
    {
        s = s + a;
        return s;
    }
    std::string &operator<< (std::string&s,const char* a)
    {
        s = s + a;
        return s;
    }
}