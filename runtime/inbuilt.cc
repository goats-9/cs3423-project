#include "inbuilt.hh"
#include "any.hh"
#include "types.hh"
#include "state.hh"
#include "helper.hh"
#include <cmath>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <limits>
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
        return ((cell *)(tab1.data))->val;
    }
    if (tab1.type == "table")
    {
        double total = 0.0;
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                double *value = (double *)(entry.second.val.data);
                total += *value;
                continue;
            }
            if (entry.second.val.type == "int")
            {
                int *value = (int *)(entry.second.val.data);
                total += double(*value);
                continue;
            }
        }
        st.outfunc();
        return any(new double(total), "double");
    }
    throw uni_err("SUM", tab1);
}

any minimum(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return ((cell *)(tab1.data))->val;
    }
    if (tab1.type == "table")
    {
        double m = std::numeric_limits<double>::max();
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                double *value = (double *)(entry.second.val.data);
                m = min(m, *value);
                continue;
            }
            if (entry.second.val.type == "int")
            {
                int *value = (int *)(entry.second.val.data);
                m = min(m, double(*value));
                continue;
            }
        }
        st.outfunc();
        return any(new double(m), "double");
    }
    throw uni_err("MINIMUM", tab1);
}

any maximum(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return ((cell *)(tab1.data))->val;
    }
    if (tab1.type == "table")
    {
        double m = std::numeric_limits<double>::min();
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                double *value = (double *)(entry.second.val.data);
                m = max(m, *value);
                continue;
            }
            if (entry.second.val.type == "int")
            {
                int *value = (int *)(entry.second.val.data);
                m = max(m, double(*value));
                continue;
            }
        }
        st.outfunc();
        return any(new double(m), "double");
    }
    throw uni_err("MAXIMUM", tab1);
}

any average(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return ((cell *)(tab1.data))->val;
    }
    if (tab1.type == "table")
    {
        table *ptr = (table *)tab1.data;
        double total = 0.0;
        int num_val = 0;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                double *value = (double *)(entry.second.val.data);
                total += *value;
                num_val++;
                continue;
            }
            if (entry.second.val.type == "int")
            {
                int *value = (int *)(entry.second.val.data);
                total += double(*value);
                num_val++;
                continue;
            }
        }
        if (!num_val)
        {
            throw runtime_error("Inoder to calculate AVERAGE, table should contains atleast one numeric entry");
        }
        double avg = total / double(num_val);
        st.outfunc();
        return any(new double(avg), "double");
    }
    throw uni_err("AVERAGE", tab1);
}

any product(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return ((cell *)(tab1.data))->val;
    }
    if (tab1.type == "table")
    {
        double prod = 1.0;
        table *ptr = (table *)tab1.data;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                double *value = (double *)(entry.second.val.data);
                prod *= *value;
                continue;
            }
            if (entry.second.val.type == "int")
            {
                int *value = (int *)(entry.second.val.data);
                prod *= double(*value);
                continue;
            }
        }
        st.outfunc();
        return any(new double(prod), "double");
    }
    throw uni_err("PRODUCT", tab1);
}

any count(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        st.outfunc();
        return any(new int(1), "int");
    }
    if (tab1.type == "table")
    {
        table *ptr = (table *)tab1.data;
        st.outfunc();
        return any(new int(ptr->tb.size()), "int");
    }
    throw uni_err("COUNT", tab1);
}

any ceiling(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        cell *ptr = (cell *)tab1.data;
        if (ptr->val.type == "double")
        {
            double val = ceil(*(double *)ptr->val.data);
            st.outfunc();
            return any(new cell(any(new double(val), "double")), "cell");
        }
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        table res;
        for (auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                res.tb[entry.first] = cell(any(new double(ceil(*(double *)entry.second.val.data)), "double"));
                continue;
            }
            res.tb[entry.first] = cell(entry.second.val);
        }
        st.outfunc();
        return any(new table(res), "table");
    }
    throw uni_err("CEILING", tab1);
}

any floor(any &tab1, const pos &p)
{
    st.infunc(p);
    if (tab1.type == "cell")
    {
        cell *ptr = (cell *)tab1.data;
        if (ptr->val.type == "double")
        {
            double val = floor(*(double *)ptr->val.data);
            st.outfunc();
            return any(new cell(any(new double(val), "double")), "cell");
        }
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        table res;
        for (auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "double")
            {
                res.tb[entry.first] = cell(any(new double(floor(*(double *)entry.second.val.data)), "double"));
                continue;
            }
            res.tb[entry.first] = cell(entry.second.val);
        }
        st.outfunc();
        return any(new table(res), "table");
    }
    throw uni_err("FLOOR", tab1);
}

any modulus(any &tab1, any &K, const pos &p)
{
    st.infunc(p);
    if (K.type != "int")
    {
        throw runtime_error("2nd argument should be int but found " + K.type);
    }
    if (tab1.type == "cell")
    {
        cell *ptr = (cell *)tab1.data;
        if (ptr->val.type == "int")
        {
            int res = *(int *)ptr->val.data % *(int *)K.data;
            st.outfunc();
            return any(new cell(any(new int(res), "int")), "cell");
        }
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        table res;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "int")
            {
                int val = (*(int *)entry.second.val.data) % (*(int *)K.data);
                res.tb[entry.first] = cell(any(new int(val), "int"));
                continue;
            }
            res.tb[entry.first] = cell(entry.second.val);
        }
        st.outfunc();
        return any(new table(res), "table");
    }
    st.outfunc();
    throw uni_err("MODULUS", tab1);
}

any power(any &tab1, any &K, const pos &p)
{
    st.infunc(p);
    if (K.type != "int" && K.type != "double")
    {
        throw runtime_error("2nd argument should be int r double but found " + K.type);
    }
    double exp;
    if (K.type == "int")
    {
        exp = *(int *)K.data;
    }
    if (K.type == "double")
    {
        exp = *(double *)K.data;
    }
    if (tab1.type == "cell")
    {
        cell *ptr = (cell *)tab1.data;
        if (ptr->val.type == "int")
        {
            int res = pow(*(int *)ptr->val.data, exp);
            st.outfunc();
            return any(new cell(any(new double(res), "double")), "cell");
        }
        if (ptr->val.type == "double")
        {
            int res = pow(*(double *)ptr->val.data, exp);
            st.outfunc();
            return any(new cell(any(new double(res), "double")), "cell");
        }
        st.outfunc();
        return tab1;
    }
    if (tab1.type == "table")
    {
        table *ptr = new table(*(table *)tab1.data);
        table res;
        for (const auto &entry : ptr->tb)
        {
            if (entry.second.val.type == "int")
            {
                int val = pow(*(int *)entry.second.val.data, exp);
                res.tb[entry.first] = cell(any(new double(val), "double"));
            }
            if (entry.second.val.type  == "double")
            {
                int val = pow(*(double *)entry.second.val.data, exp);
                res.tb[entry.first] = cell(any(new double(val), "double"));
            }
            res.tb[entry.first] = cell(entry.second.val);
        }
        st.outfunc();
        return any(new table(res), "table");
    }
    st.outfunc();
    throw uni_err("POWER", tab1);
}