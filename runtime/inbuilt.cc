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
    st.infunc(p);
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

/* Bitwise starts */

any BOR(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond) throw bi_err("BOR", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) | (*(int *)b.data)), "int");
}

any BAND(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond) throw bi_err("BAND", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) && (*(int *)b.data)), "int");
}

any BXOR(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond) throw bi_err("BXOR", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) ^ (*(int *)b.data)), "int");
}

any BNOR(const any &a, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int");
    if (!cond) throw uni_err("BNOR", a);
    st.outfunc();
    return any(new int(~(*(int *)a.data)), "int");
}

any BLS(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond) throw bi_err("BLS", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) << (*(int *)b.data)), "int");
}

any BRS(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond) throw bi_err("BRS", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) >> (*(int *)b.data)), "int");
}

/* Bitwise ends */

// TYPEOF
any TYPEOF(const any &a,const pos &p)
{
    st.infunc(p);
    st.outfunc();
    return any(new string(a.type),"string");
}

// DISP
any DISP(const any &a,const pos &p)
{
    // if (a.type == "array")
    // {
    //     vector<any> *ptr = (vector<any> *)a.data;
    //     for(any i: (*ptr))
    //     {
    //         cout << i << " " ;
    //     }
    //     cout << "\n";
    //     return any();
    // }
    st.infunc(p);
    std::cout << a << "\n";
    st.outfunc();
    return any();
}

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

// any sum(any &tab1, const pos &p)
// {
//     st.infunc(p);
//     if(tab1.type == "cell")
//     {
//         double *ptr1 = (double *)tab1.data;
//         st.outfunc();
//         return any(new double(*ptr1), "double");
//     }
//     if(tab1.type == "table")
//     {
//         double total = 0.0;
//         for (const auto &entry : tab1.data.tb){
//             double value = (double *)entry.second.val;
//             total += value;
//         }
//         st.outfunc();
//         return any(new double(total), "double");
//     }
//     st.outfunc();
//     throw uni_error("SUM",tab1);
// }
any minimum(any &tab1);
any maximum(any &tab1);
any average(any &tab1);
any product(any &tab1);
any count(any &tab1);
any ceiling(any &tab1);
any floor(any &tab1);
any modulus(any &tab1, any &N);
any power(any &tab1, any &N);