#include "any.hh"
#include "helper.hh"
#include "state.hh"
#include <stdexcept>
using namespace std;

extern state st;

// unary function error message
runtime_error uni_err(const std::string &func, const any &a)
{
    return runtime_error(func + " does not support (" + a.type + ")" );
}

// binary function error message
runtime_error bi_err(const string &func, const any &a, const any &b)
{
    return runtime_error(func + " does not support (" + a.type + ", " + b.type + ")");
}

vector<string> split(string s, string del)
{
    std::vector<std::string> res;
    int start, end = -1 * del.size();
    do
    {
        start = end + del.size();
        end = s.find(del, start);
        res.push_back(s.substr(start, end - start));
    } while (end != -1);

    return res;
}

int to_int(const any &a,const pos & p)
{
    st.infunc(p);
    if (a.type == "int")
    {
        st.outfunc();
        return *(int *)a.data;
    }
    if (a.type == "bool")
    {
        st.outfunc();
        return int(*(bool *)a.data);
    }
    if (a.type == "double")
    {
        st.outfunc();
        return int(*(double *)a.data);
    }
    throw std::runtime_error("Return type of main is not int (but " + a.type + ")");
}

int to_bool(const any &a,const pos & p)
{
    st.infunc(p);
    if (a.type == "int")
    {
        st.outfunc();
        return bool(*(int *)a.data);
    }
    if (a.type == "double")
    {
        st.outfunc();
        return bool(*(double *)a.data);
    }
    if (a.type == "bool")
    {
        st.outfunc();
        return (*(bool *)a.data);
    }
    throw std::runtime_error("Return type of main is not int(but " + a.type + ")");
}

void disp_error(const runtime_error &e)
{
    cerr 
    << "Runtime Type error:\n"
    << st
    << e.what() << '\n';
}