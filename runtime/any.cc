#include "any.hh"
#include "types.hh"
#include "helper.hh"
#include "state.hh"
#include <iostream>
#include <vector>
using namespace std;

extern state st;

bool any::isInbuilt(const string &t)
{
    vector<string> primitive{
        "int", 
        "double", 
        "string", 
        "bool", 
        "none", 
        "array", 
        "cell", 
        "shape", 
        "range", 
        "date", 
        "time"
    };
    for (auto i : primitive)
    {
        if (i == t)
        {
            return true;
        }
    }
    return false;
}

void any::construct(const any &a)
{
    if (a.type == "int")
    {
        data = new int(*(int *)a.data);
        type = "int";
    }
    if (a.type == "double")
    {
        data = new double(*(double *)a.data);
        type = "double";
    }
    if (a.type == "string")
    {
        data = new string(*(string *)a.data);
        type = "string";
    }
    if (a.type == "bool")
    {
        data = new bool(*(bool *)a.data);
        type = "bool";
    }
    if (a.type == "none")
    {
        data = NULL;
        type = "none";
    }
    if (a.type == "array")
    {
        data = new vector<any>(*(vector<any> *)a.data);
        type = "array";
    }
    if (a.type == "cell")
    {
        data = new cell(*(cell *)a.data);
        type = "cell";
    }
    if (a.type == "shape")
    {
        data = new shape(*(shape *)a.data);
        type = "shape";
    }
    if (a.type == "range")
    {
        data = new range(*(range *)a.data);
        type = "range";
    }
    if (a.type == "date")
    {
        data = new date(*(date *)a.data);
        type = "date";
    }
    if (a.type == "time")
    {
        data = new Time(*(Time *)a.data);
        type = "time";
    }
    // if (a.type == "table")
    // {
    //     data = new table(*(table *)a.data);
    // }
    Constructor(a);
}

void any::destruct()
{
    if (type == "int")
    {
        delete (int *)data;
    }
    if (type == "double")
    {
        delete (double *)data;
    }
    if (type == "string")
    {
        delete (string *)data;
    }
    if (type == "bool")
    {
        delete (bool *)data;
    }
    if (type == "none")
    {
        // nothing
    }
    if (type == "array")
    {
        delete (vector<any> *)data;
    }
    if (type == "cell")
    {
        delete (cell *)data;
    }
    if (type == "shape")
    {
        delete (shape *)data;
    }
    if (type == "range")
    {
        delete (range *)data;
    }
    if (type == "date")
    {
        delete (date *)data;
    }
    if (type == "time")
    {
        delete (Time *)data;
    }
    // if (type == "table")
    // {
    //     delete (table *)data;
    // }
    Destructor();
}

// access member
any &any::access(const string &id, const pos &p)
{
    st.infunc(p);
    if (isInbuilt(type))
    {
        throw runtime_error(type + " does not have any member " + id);
    }
    st.outfunc();
    return Accessor(id);
}
// run method
any any::run(const string &id, const vector<any> &params, const pos &p)
{
    st.infunc(p);
    if (type == "array")
    {
        if (id == "size")
        {
            if (params.size() != 0)
            {
                throw runtime_error(id + " accepts 0 params but recieved " + to_string(params.size()));
            }
            st.outfunc();
            return any(new int(((vector<any> *)data)->size()), "int");
        }
        if (id == "add_item")
        {
            if (params.size() != 1)
            {
                throw runtime_error(id + " accepts 1 params but recieved " + to_string(params.size()));
            }
            ((vector<any> *)data)->push_back(params[0]);
            st.outfunc();
            return any();
        }
        throw runtime_error(type + " does not have any method " + id);
    }
    if (isInbuilt(type))
    {
        throw runtime_error(type + " does not have any method " + id);
    }
    return Runner(id, params, p);
}

any &any::at(const any &i, const pos &p)
{
    st.infunc(p);
    if (type == "array")
    {
        if (i.type != "int")
        {
            throw runtime_error("index of array should be a integer but it was " + i.type);
        }
        vector<any> *ptr = (vector<any> *)data;
        int idx = *(int *)i.data;
        if (idx >= (int)ptr->size() || idx < 0)
        {
            throw runtime_error("out of index");
        }
        st.outfunc();
        return (*ptr)[idx];
    }
    // if (type == "table")
    // {
    //     if (i.type != "shape")
    //     {
    //         throw runtime_error("index of table should be a shape but it was " + i.type);
    //     }
    //     table *ptr = (table *)data;
    //     return (*ptr)[i];
    // }
    throw runtime_error("[] operator does not support (" + type + ")");
}

/* operator overloading starts */

// == operator
bool operator==(const any &a, const any &b)
{
    if (a.type != b.type)
    {
        return false;
    }
    if (a.type == "int")
    {
        bool val = (*(int *)a.data) == (*(int *)b.data);
        return val;
    }
    if (a.type == "double")
    {
        bool val = (*(double *)a.data) == (*(double *)b.data);
        return val;
    }
    if (a.type == "bool")
    {
        bool val = (*(bool *)a.data) == (*(bool *)b.data);
        return val;
    }
    if (a.type == "string")
    {
        bool val = (*(string *)a.data) == (*(string *)b.data);
        return val;
    }
    if (a.type == "none")
    {
        bool val = true;
        return val;
    }
    if (a.type == "array")
    {
        bool val = (*(vector<any> *)a.data) == (*(vector<any> *)b.data);
        return val;
    }
    throw bi_err("==", a, b);
}

// != operator
bool operator!=(const any &a, const any &b)
{
    try
    {
        return !(a == b);
    }
    catch (const runtime_error &e)
    {
        throw bi_err("!=", a, b);
    }
}

// > operator
bool operator>(const any &a, const any &b)
{
    if (a.type != b.type)
    {
        throw bi_err(">", a, b);
    }
    if (a.type == "int")
    {
        bool val = (*(int *)a.data) > (*(int *)b.data);
        return val;
    }
    if (a.type == "double")
    {
        bool val = (*(double *)a.data) > (*(double *)b.data);
        return val;
    }
    if (a.type == "bool")
    {
        bool val = (*(bool *)a.data) > (*(bool *)b.data);
        return val;
    }
    throw bi_err(">", a, b);
}

// >= operator
bool operator>=(const any &a, const any &b)
{
    try
    {
        return (a > b || a == b);
    }
    catch (const runtime_error &e)
    {
        throw bi_err(">=", a, b);
    }
}

// < operator
bool operator<(const any &a, const any &b)
{
    try
    {
        return !(a >= b);
    }
    catch (const runtime_error &e)
    {
        throw bi_err("<", a, b);
    }
}

// <= operator
bool operator<=(const any &a, const any &b)
{
    try
    {
        return !(a > b);
    }
    catch (const runtime_error &e)
    {
        throw bi_err("<=", a, b);
    }
}

// << operator
ostream &operator<<(ostream &o, const any &a)
{
    if (a.type == "int")
    {
        o << *(int *)a.data;
        return o;
    }
    if (a.type == "string")
    {
        o << *(string *)a.data;
        return o;
    }
    if (a.type == "double")
    {
        o << *(double *)a.data;
        return o;
    }
    if (a.type == "bool")
    {
        o << (*(bool *)a.data ? "true" : "false");
        return o;
    }
    if (a.type == "none")
    {
        o << "none";
        return o;
    }
    if (a.type == "range")
    {
        range *ptr = (range *)a.data;
        o << ptr->start << ":" << ptr->stop << "~" << ptr->step;
        return o;
    }
    if (a.type == "time")
    {
        Time *ptr = (Time *)a.data;
        o << ptr->hour << ":" << ptr->min << ":" << ptr->sec;
        return o;
    }
    if (a.type == "date")
    {
        date *ptr = (date *)a.data;
        o << ptr->year << "-" << ptr->month << "-" << ptr->day;
        return o;
    }
    if (a.type == "shape")
    {
        shape *ptr = (shape *)a.data;
        o << "(" << ptr->vals.first << "," << ptr->vals.second << ")" ;
        return o;
    }
    if (a.type == "array")
    {
        vector<any> *ptr = (vector<any> *)a.data;
        if (ptr->size() == 0)
        {
            o << "[]";
            return o;
        }
        o << "[";
        for (int i = 0; i < (int)ptr->size() - 1; i++)
        {
            o << (*ptr)[i] << ", ";
        }
        o << (*ptr)[ptr->size() - 1] << "]";
        return o;
    }
    throw uni_err("DISP", a);
    ;
}

/* operator overloading ends */