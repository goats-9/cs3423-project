#include "any.hh"
#include "helper.hh"
#include <stdexcept>
using namespace std;

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