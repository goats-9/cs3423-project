#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>

class any;
// /**
//  * @class shape
// */
// class shape
// {
// public:
//     std::pair<int, int> vals;
//     shape();
//     shape(any &a, any &b);
// };

// /**
//  * @class range
// */
// class range 
// {
// public:
//     int start, stop, step;
//     range (int _start, int _stop, int _step) 
//         : start(_start), stop(_stop), step(_step)
//         {}
// };
class cell
{
private:
    void destroy();
    void construct(const cell &a);
    void construct(const any &a);

public:
    any *val;
    bool is_first;
    // default constructor
    cell() : is_first(true)
    {
        val = NULL;
    }
    // copy constructor
    cell(const cell &a)
    {
        construct(a);
    }
    // constructor with any
    cell(const any &a) : is_first(true)
    {
        construct(a);
    }

    // assignment operator
    void operator=(const cell &a)
    {
        destroy();
        construct(a);
    }

    // get double value from cell
    // double getDoubleVal() {
    //     std::string type = val->type;
    //     if (type == "int") {
    //         return static_cast<double>(*(reinterpret_cast<int*>(val->data)));
    //     } else if (type == "double") {
    //         return *(reinterpret_cast<double*>(val->data));
    //     } else {
    //         return 0.0;
    //     }
    // }

    // destructor
    ~cell()
    {
        destroy();
    }
};

// class table
// {
// public:
//     std::map<std::pair<int, int>, cell> tb;
//     shape sp;
//     // default constructor
//     table()
//     {
//         // empty
//     }
//     // copy constructor
//     table(const table &a)
//     {
//         tb = a.tb;
//     }
//     any operator[](any &dim);
//     int read(any &path, any &delim);
//     int write(any &path, any &delim);
//     any shape();
// };


// class date
// {
// public:
//     int year, month, day;
//     date(std::string str);
// };

// class time
// {
// public:
//     int hour, min, sec;
//     time(std::string str);
// };
