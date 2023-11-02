#include "any.hh"
#include "helper.hh"
#include <stdexcept>
using namespace std;

// unary function error message
std::runtime_error uni_err(const std::string &func, const any &a)
{
    return runtime_error(func + " does not support (" + a.type + ")" );
}

// binary function error message
runtime_error bi_err(const string &func, const any &a, const any &b)
{
    return runtime_error(func + " does not support (" + a.type + ", " + b.type + ")");
}