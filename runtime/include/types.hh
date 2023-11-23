#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>

class any;
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
    double getDoubleVal() {
        std::string type = val->type;
        if (type == "int") {
            return static_cast<double>(*(reinterpret_cast<int*>(val->data)));
        } else if (type == "double") {
            return *(reinterpret_cast<double*>(val->data));
        } else {
            return 0.0;
        }
    }

    // destructor
    ~cell()
    {
        destroy();
    }
};

class table
{
public:
    std::map<std::pair<int, int>, cell> tb;
    int row_max, col_max;
    // default constructor
    table()
    {
        // empty
    }
    // copy constructor
    table(const table &a)
    {
        tb = a.tb;
    }
    int read(std::string &path, char delim);
    int write(std::string &path, char delim);
    any sum(any &r1, any &r2);
    any minimum(any &r1, any &r2);
    any maximum(any &r1, any &r2);
    any product(any &r1, any &r2);
    any average(any &r1, any &r2);
    any count(any &r1, any &r2);
    any ceiling(any &r1, any &r2);
    any floor(any &r1, any &r2);
    any modulus(any &r1, any &r2, any &r3);
    any power(any &r1, any &r2, any &r3);
    any shape();
};

class date
{
public:
    int year, month, day;
    date(std::string str);
};

class time
{
public:
    int hour, min, sec;
    time(std::string str);
};

/**
 * @class formula
*/
class formula
{
public:
    table tb;
    std::string opname;
    std::vector<any> args;
    formula (table &_tb, std::string &_opname, std::vector<any> &_args) 
            : tb(_tb), opname(_opname), args(_args)
            {}
};

/**
 * @class range
*/
class range 
{
public:
    int start, stop, step;
    range (int _start, int _stop, int _step) 
        : start(_start), stop(_stop), step(_step)
        {}
};