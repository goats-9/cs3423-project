#include "inbuilt.hh"
#include "any.hh"
#include "types.hh"
#include "state.hh"
#include "helper.hh"
#include <cmath>
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;

extern state st;

// Inbuilt function

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
any EQ(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a == b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Not Equal
any NE(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a != b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Greater Than
any GT(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a > b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Less Than
any LT(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a < b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Greater Than or Equal
any GTE(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a >= b);
    st.outfunc();
    return any(new bool(val), "bool");
}

// Less Than or Equal
any LTE(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool val = (a <= b);
    st.outfunc();
    return any(new bool(val), "bool");
}

/* Comparison ends */

// DISP
any DISP(const any &a)
{
    if (a.type == "array")
    {
        vector<any> *ptr = (vector<any> *)a.data;
        for(any i: (*ptr))
        {
            cout << i << " " ;
        }
        cout << "\n";
        return any();
    }
    throw uni_err("DISP",a);
}