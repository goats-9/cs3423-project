#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>

class any;
/**
 * @class shape
*/
class shape
{
public:
    std::pair<int, int> vals;
    shape();
    shape(any &a, any &b);
};
#include "any.hh"

/**
 * @class shape
*/
class shape
{
public:
    std::pair<int, int> vals;
    shape(const any &a, const any &b,const pos &p);
};

/**
 * @class range
*/
class range 
{
public:
    int start, stop, step;
    range (std::string str,const pos &p);
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
};

class table
{
public:
    std::map<std::pair<int, int>, cell> tb;
    // shape sp;
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
    any operator[](any &dim);
    int read(any &path, any &delim);
    int write(any &path, any &delim);
    any dim();
};


class date
{
public:
    int year, month, day;
    date(std::string str,const pos &p);
};

class Time
{
public:
    int hour, min, sec;
    Time(std::string str,const pos &p);
};
