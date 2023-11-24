#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <vector>
#include "any.hh"

// for translation of classes
template <typename T>
using __funcMap = std::map<std::string, any (T::*)(std::vector<any>, const pos &)>;
using __funcParams = std::map<std::string, int>;
using __memMap = std::map<std::string, any>;

/**
 * @class shape
*/
class shape
{
public:
    std::pair<any, any> vals;
    shape(const any &a, const any &b,const pos &p);
    // for internal use
    shape(const any &a, const any &b);
    shape(){}
};

/**
 * @class range
*/
class range 
{
public:
    int start, stop, step;
    range (std::string str,const pos &p);
    range (const any &a);
    range(){}
};
class cell
{
private:
    void destroy();
    void construct(const cell &a);
    void construct(const any &a);

public:
    any val;
    // default constructor
    cell()
    {
        val = any();
    }
    // copy constructor
    cell(const cell &a)
    {
        construct(a);
    }
    // constructor with any
    cell(const any &a)
    {
        construct(a);
    }

    // assignment operator
    void operator=(const cell &a)
    {
        destroy();
        construct(a);
    }

    // destructor
    ~cell()
    {
        destroy();
    }
    any get();
};

class table
{
public:
    std::map<std::pair<int, int>, cell> tb;
    int max_row,max_col;
    // default constructor
    table() : max_row(0), max_col(0)
    {
        // empty
    }
    // copy constructor
    table(const table &a)
    {
        tb = a.tb;
        max_row = a.max_row;
        max_col = a.max_col;
    }
    any assign(const any &shape, const any &elements);
    any read(const any &path, const any &delim);
    any write(const any &path, const any &delim);
    any dim();
    any get(const any &dim);
};


class date
{
public:
    int year, month, day;
    date(std::string str,const pos &p);
    date(){}
};

class Time
{
public:
    int hour, min, sec;
    Time(std::string str,const pos &p);
    Time(){}
};
