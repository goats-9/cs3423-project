#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <utility>

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
    void read(std::string &path, std::string delim);
    void write(std::string &path, std::string delim);
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