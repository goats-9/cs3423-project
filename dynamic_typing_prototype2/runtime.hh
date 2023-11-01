#ifndef RUNTIME_HH
#define RUNTIME_HH

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <cmath>
#include "state.hh"
using namespace std;

state st;

class any;

class any
{
private:
    inline void Constructor(const any &a);
    inline void Destructor();
    inline any &Accessor(const string &id);
    inline any Runner(const string &id, const vector<any> &params);
    void construct(const any &a)
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
        Constructor(a);
    }

    void destruct()
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
        Destructor();
    }

    vector<string> primitive{"int", "double", "string", "bool", "none", "array"};

    bool isInbuilt(const string &t)
    {
        for (auto i : primitive)
        {
            if (i == t)
            {
                return true;
            }
        }
        return false;
    }

public:
    void *data;
    string type;
    // default constructor
    any()
    {
        this->data = NULL;
        this->type = "none";
    }
    // constructor for compiler
    any(void *data, const string &type)
    {
        this->data = data;
        this->type = type;
    }
    // copy constructor
    any(const any &a)
    {
        construct(a);
    }
    // assignment operator
    void operator=(const any &a)
    {
        destruct();
        construct(a);
    }
    // access member
    any access(const string &id)
    {
        if (isInbuilt(type))
        {
            throw runtime_error(type + " does not have any member " + id);
        }
        Accessor(id);
    }
    // run method
    any run(const string &id, const vector<any> &params)
    {
        if (isInbuilt(type))
        {
            throw runtime_error(type + " does not have any method " + id);
        }
        Runner(id, params);
    }
    // accessing array elements
    any &at(const any &i)
    {
        if (i.type != "int")
        {
            throw runtime_error("index should be a integer but it was " + i.type);
        }
        if (type != "array")
        {
            throw runtime_error("accesing a non-array (" + type + ")");
        }
        vector<any> *ptr = (vector<any> *)data;
        int idx = *(int *)i.data;
        if (idx >= ptr->size())
        {
            throw runtime_error("out of index");
        }
        return (*ptr)[idx];
    }
    ~any()
    {
        destruct();
    }
};
// Inbuilt function

// binary function error message
runtime_error bi_err(const string &func, const any &a, const any &b)
{
    return runtime_error(func + "does not support (" + a.type + ", " + b.type + ")");
}

/* operator overloading starts */

// == operator
bool operator==(const any &a,const any &b)
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
    throw bi_err("==",a,b);
}
// != operator
bool operator!=(const any &a,const any &b)
{
    try
    {
        return !(a == b);
    }
    catch(const runtime_error& e)
    {
        throw bi_err("!=",a,b);
    }
}
// > operator
bool operator>(const any &a,const any &b)
{
    if (a.type != b.type)
    {
        throw bi_err(">",a,b);
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
    throw bi_err(">",a,b);
}
// >= operator
bool operator>=(const any &a,const any &b)
{
    try
    {
        return (a > b || a == b);
    }
    catch(const runtime_error& e)
    {
        throw bi_err(">=",a,b);
    }
}
// < operator
bool operator<(const any &a,const any &b)
{
    try
    {
        return !(a >= b);
    }
    catch(const runtime_error& e)
    {
        throw bi_err("<",a,b);
    }
}
// <= operator
bool operator<=(const any &a,const any &b)
{
    try
    {
        return !(a > b);
    }
    catch(const runtime_error& e)
    {
        throw bi_err("<=",a,b);
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
        o << *(bool *)a.data ? "true" : "false";
        return o;
    }
    if (a.type == "none")
    {
        o << "none";
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
        for (int i = 0; i < ptr->size() - 1; i++)
        {
            o << (*ptr)[i] << ", ";
        }
        o << (*ptr)[ptr->size() - 1] << "]";
        return o;
    }
    throw runtime_error(" DISP does not support " + a.type);
}

/* operator overloading ends */

/* Arithmetic starts */

// ADD
any ADD(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    if (a.type == "int" && b.type == "int")
    {
        int *ptr1 = (int *)a.data;
        int *ptr2 = (int *)b.data;
        st.outfunc();
        return any(new int(*ptr1 + *ptr2), "int");
    }
    if (
        (a.type == "double" && b.type == "int") ||
        (a.type == "int" && b.type == "double") ||
        (a.type == "double" && b.type == "double"))
    {
        double *ptr1 = (double *)a.data;
        double *ptr2 = (double *)b.data;
        st.outfunc();
        return any(new double(*ptr1 + *ptr2), "double");
    }
    throw bi_err("ADD", a, b);
}

// SUB
any SUB(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    if (a.type == "int" && b.type == "int")
    {
        int *ptr1 = (int *)a.data;
        int *ptr2 = (int *)b.data;
        st.outfunc();
        return any(new int(*ptr1 - *ptr2), "int");
    }
    if (
        (a.type == "double" && b.type == "int") ||
        (a.type == "int" && b.type == "double") ||
        (a.type == "double" && b.type == "double"))
    {
        double *ptr1 = (double *)a.data;
        double *ptr2 = (double *)b.data;
        st.outfunc();
        return any(new double(*ptr1 - *ptr2), "double");
    }
    throw bi_err("SUB", a, b);
}

// MUL
any MUL(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    if (a.type == "int" && b.type == "int")
    {
        int *ptr1 = (int *)a.data;
        int *ptr2 = (int *)b.data;
        st.outfunc();
        return any(new int(*ptr1 * *ptr2), "int");
    }
    if (
        (a.type == "double" && b.type == "int") ||
        (a.type == "int" && b.type == "double") ||
        (a.type == "double" && b.type == "double"))
    {
        double *ptr1 = (double *)a.data;
        double *ptr2 = (double *)b.data;
        st.outfunc();
        return any(new double(*ptr1 * *ptr2), "double");
    }
    throw bi_err("MUL", a, b);
}

// DIV
any DIV(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    if (
        (a.type == "int" || a.type == "double") &&
        (b.type == "int" || b.type == "double"))
    {
        double *ptr1 = (double *)a.data;
        double *ptr2 = (double *)b.data;
        if (*ptr2 == 0)
        {
            throw runtime_error("division by 0");
        }
        st.outfunc();
        return any(new double(*ptr1 / *ptr2), "double");
    }
    throw bi_err("DIV", a, b);
}

// MOD
any MOD(const any &a, const any &b, const pos &p)
{
    if (a.type == "int" && b.type == "int")
    {
        int *ptr1 = (int *)a.data;
        int *ptr2 = (int *)b.data;
        st.outfunc();
        return any(new int(*ptr1 % *ptr2), "int");
    }
    throw bi_err("MOD", a, b);
}

// POW
any POW(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    if (a.type == "int" && b.type == "int")
    {
        int *ptr1 = (int *)a.data;
        int *ptr2 = (int *)b.data;
        st.outfunc();
        return any(new int((int)pow(*ptr1, *ptr2)), "int");
    }
    if (
        (a.type == "double" && b.type == "int") ||
        (a.type == "int" && b.type == "double") ||
        (a.type == "double" && b.type == "double"))
    {
        double *ptr1 = (double *)a.data;
        double *ptr2 = (double *)b.data;
        st.outfunc();
        return any(new double(pow(*ptr1, *ptr2)), "double");
    }
    throw bi_err("POW", a, b);
}

/* Arithmetic ends */

/* Comparison starts */

// Equal
any EQ(any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a == b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Not Equal
any NE(any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a != b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Greater Than
any GT(any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a > b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Less Than
any LT(any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a < b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Greater Than or Equal
any GTE(any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a >= b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Less Than or Equal
any LTE(any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a <= b);
    st.outfunc();
    return any(new bool(val), "bool");
}

/* Comparison ends */

// DISP


#endif