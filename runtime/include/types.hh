#ifndef TYPES_HH
#define TYPES_HH

class any;
#include <vector>

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
    std::vector<std::vector<cell>> tb;
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
};

#endif