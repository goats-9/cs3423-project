#pragma once
#include "inbuilt.hh"
#include "helper.hh"
#include "any.hh"
#include "types.hh"
#include "state.hh"
#include <map>
#include <stdexcept>
using namespace std;

template <typename T>
using funcMap = map<string, any (T::*)(vector<any>, const pos &)>;
using funcParams = map<string, int>;
using memMap = map<string, any>;

#define copyConstruct \
    func_decl();      \
    mem = a.mem;      \
    func = a.func;    \
    func_params = a.func_params;

#define runnerCode                                                                                                         \
    f = ptr->func[id];                                                                                                     \
    if (!f)                                                                                                                \
    {                                                                                                                      \
        throw runtime_error(type + " does not have any method " + id);                                                     \
    }                                                                                                                      \
    if (ptr->func_params[id] != params.size())                                                                             \
    {                                                                                                                      \
        string err;                                                                                                        \
        err = id + " accepts " + to_string(ptr->func_params[id]) + " parameters but recieved " + to_string(params.size()); \
        throw runtime_error(err);                                                                                          \
    }                                                                                                                      \
    st.outfunc();                                                                                                          \
    return (ptr->*f)(params, p);
