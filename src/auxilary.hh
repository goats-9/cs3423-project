#ifndef AUXILARY_HH
#define AUXILARY_HH
#include <string>
#include <vector>
using namespace std;

namespace helper
{
    // define helping functions and classes here

    // splits the string given delimiter
    vector<string> split(string s, string del);

    class date
    {
    public:
        int year;
        int month;
        int day;
        date();
        date(string str);
    };

    class time
    {
    public:
        int hour;
        int min;
        int sec;
        time();
        time(string str);
    };
}

#endif