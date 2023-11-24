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
    if (!cond)
        throw bi_err("BOR", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) | (*(int *)b.data)), "int");
}

any BAND(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond)
        throw bi_err("BAND", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) && (*(int *)b.data)), "int");
}

any BXOR(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond)
        throw bi_err("BXOR", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) ^ (*(int *)b.data)), "int");
}

any BNOR(const any &a, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int");
    if (!cond)
        throw uni_err("BNOR", a);
    st.outfunc();
    return any(new int(~(*(int *)a.data)), "int");
}

any BLS(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond)
        throw bi_err("BLS", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) << (*(int *)b.data)), "int");
}

any BRS(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    bool cond = (a.type == "int") && (b.type == "int");
    if (!cond)
        throw bi_err("BRS", a, b);
    st.outfunc();
    return any(new int((*(int *)a.data) >> (*(int *)b.data)), "int");
}

/* Bitwise ends */

// TYPEOF
any TYPEOF(const any &a, const pos &p)
{
    st.infunc(p);
    st.outfunc();
    return any(new string(a.type), "string");
}

// DISP
any DISP(const any &a, const pos &p)
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

any sum(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        double total = 0.0;
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            double *value = (double *)entry.second.val;
            total += *value;
        }
        st.outfunc();
        return any(new cell(any(new double(total), "double")), "cell");
    }
    st.outfunc();
    throw uni_err("SUM", tab1);
}

any minimum(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        double m = std::numeric_limits<double>::max();
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            double *value = (double *)entry.second.val;
            m += min(m, *value);
        }
        st.outfunc();
        return any(new cell(any(new double(m), "double")), "cell");
    }
    st.outfunc();
    throw uni_err("MINIMUM", tab1);
}

any maximum(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        double m = std::numeric_limits<double>::min();
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            double *value = (double *)entry.second.val;
            m += max(m, *value);
        }
        st.outfunc();
        return any(new cell(any(new double(m), "double")), "cell");
    }
    st.outfunc();
    throw uni_err("MAXIMUM", tab1);
}

any average(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        table *ptr = (table *)tab1.data;
        double total = 0.0;
        for (const auto &entry : ptr->tb)
        {
            double *value = (double *)entry.second.val;
            total += *value;
        }
        st.outfunc();
        double avg = total / ptr->tb.size();
        return any(new cell(any(new double(avg), "double")), "cell");
    }
    st.outfunc();
    throw uni_err("AVERAGE", tab1);
}

any product(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        double prod = 1.0;
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            double *value = (double *)entry.second.val;
            prod *= *value;
        }
        st.outfunc();
        return any(new cell(any(new double(prod), "double")), "cell");
    }
    st.outfunc();
    throw uni_err("PRODUCT", tab1);
}

any count(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        table *ptr = (table *)tab1.data;
        st.outfunc();
        return any(new cell(any(new int(ptr->tb.size()), "int")), "cell");
    }
    st.outfunc();
    throw uni_err("COUNT", tab1);
}

any ceiling(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        cell *ptr = new cell(*(cell *)tab1.data);
        *(double *)ptr->val = ceil(*(double *)ptr->val);
        st.outfunc();
        return any(ptr, "cell");
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        for (const auto &entry : ptr->tb)
        {
            *(double *)entry.second.val = ceil(*(double *)entry.second.val);
        }
        st.outfunc();
        return any(ptr, "table");
    }
    st.outfunc();
    throw uni_err("CEILING", tab1);
}

any floor(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        cell *ptr = new cell(*(cell *)tab1.data);
        *(double *)ptr->val = floor(*(double *)ptr->val);
        st.outfunc();
        return any(ptr, "cell");
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        for (const auto &entry : ptr->tb)
        {
            *(double *)entry.second.val = floor(*(double *)entry.second.val);
        }
        st.outfunc();
        return any(ptr, "table");
    }
    st.outfunc();
    throw uni_err("FLOOR", tab1);
}

any modulus(any &tab1, any &K, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        cell *ptr = new cell(*(cell *)tab1.data);
        *(int *)ptr->val = (*(int *)ptr->val) % (*(int *)K.data);
        st.outfunc();
        return any(ptr, "cell");
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        for (const auto &entry : ptr->tb)
        {
            *(int *)entry.second.val = (*(int *)entry.second.val) % (*(int *)K.data);
        }
        st.outfunc();
        return any(ptr, "table");
    }
    st.outfunc();
    throw uni_err("MODULUS", tab1);
}

any power(any &tab1, any &K, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        cell *ptr = new cell(*(cell *)tab1.data);
        *(double *)ptr->val = pow(*(double *)ptr->val, *(double *)K.data);
        st.outfunc();
        return any(ptr, "cell");
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        for (const auto &entry : ptr->tb)
        {
            *(double *)entry.second.val = pow(*(double *)entry.second.val, *(double *)K.data);
        }
        st.outfunc();
        return any(ptr, "table");
    }
    st.outfunc();
    throw uni_err("POWER", tab1);
}