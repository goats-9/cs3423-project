#ifndef RUNTIME_HH
#define RUNTIME_HH

#define ACCESS_CODE                                 \
    if (!(p->mem[id].ptr))                          \
    {                                               \
        p->mem[id].ptr = (ptr_gen *)new ptr_none(); \
    }                                               \
    st.outfunc();                                   \
    return p->mem[id];
#define RUN_CODE                                                                \
    func_ptr = p->func[id];                                                     \
    if (func_ptr && p->params[id] == params.size())                             \
    {                                                                           \
        return (p->*func_ptr)(params);                                          \
    }                                                                           \
    if (!(func_ptr))                                                            \
    {                                                                           \
        throw runtime_error("'" + id + "'" + " function not found in " + type); \
    }                                                                           \
    throw runtime_error("parameters of '" + id + "' (" + to_string(params.size()) + " parameters)" + " does not match the expected parameters (" + to_string(p->params[id]) + " parameters)");

#include <string>
#include <stdexcept>
#include "state.hh"
#include <map>
#include <vector>
#include <cmath>
using namespace std;

class ptr_gen;
state st;

// smart pointer
class any
{
public:
    // pointer to general type
    ptr_gen *ptr;

    // default contructor
    any()
    {
        ptr = NULL;
    }
    // contructor with pointer
    any(ptr_gen *ptr)
    {
        this->ptr = ptr;
    }
    // copy constructor
    any(const any &a);
    // assignment
    void operator=(const any &a);
    // accesing any function of gen
    ptr_gen *operator->()
    {
        return ptr;
    }
    // destructor
    ~any();
};

class ptr_gen
{
public:
    // type of variable
    string type;
    // data stored
    void *val;

    // gets deep copy
    virtual ptr_gen *deepCopy() = 0;
    // accessing a member
    virtual any &access(const string &id, const pos &p)
    {
        st.infunc(p);
        throw runtime_error(id + " is not a member of " + type);
    }
    // running a function with name id
    virtual any run(const string &id, const vector<any> &params, const pos &p)
    {
        st.infunc(p);
        throw runtime_error(id + " is not a method of " + type);
    }
    // accessing i indexed element of array
    virtual any &at(const any &i, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("Not an array");
    }
    // add item
    virtual void add_item(const any &item, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("Not an array");
    }
    // size
    virtual int size(const pos &p)
    {
        st.infunc(p);
        throw runtime_error("Not an array or string");
    }
    // ADD
    virtual any ADD(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    // SUB
    virtual any SUB(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("SUB does not support: (" + type + "," + a.ptr->type + ")");
    }
    // MUL
    virtual any MUL(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("MUL does not support: (" + type + "," + a.ptr->type + ")");
    }
    // DIV
    virtual any DIV(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("DIV does not support: (" + type + "," + a.ptr->type + ")");
    }
    // POW
    virtual any POW(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("POW does not support: (" + type + "," + a.ptr->type + ")");
    }
    // MOD
    virtual any MOD(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("MOD does not support: (" + type + "," + a.ptr->type + ")");
    }
    // CONCAT
    virtual any CONCAT(const any &a, const pos &p)
    {
        st.infunc(p);
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    // destructor
    virtual ~ptr_gen(){};
};

class ptr_none : public ptr_gen
{
public:
    ptr_none()
    {
        type = "none";
        this->val = NULL;
    }
    ptr_none *deepCopy()
    {
        return new ptr_none();
    }
};

class ptr_string : public ptr_gen
{
public:
    ptr_string(const string &val)
    {
        type = "string";
        this->val = new string(val);
    }
    ptr_string *deepCopy()
    {
        return new ptr_string(*(string *)val);
    }
    int size(const pos &p)
    {
        st.infunc(p);
        string *ptr = (string *)val;
        int sz = ptr->size();
        st.outfunc();
        return sz;
        // return any(new ptr_int(sz));
    }
    any CONCAT(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "string")
        {
            string val = *(string *)(this->val) + *((string *)a.ptr->val);
            st.outfunc();
            return any(new ptr_string(val));
        }
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    ~ptr_string()
    {
        delete (string *)val;
    }
};

class ptr_double : public ptr_gen
{
public:
    ptr_double(const double &val)
    {
        type = "double";
        this->val = new double(val);
    }
    ptr_double *deepCopy()
    {
        return new ptr_double(*(double *)val);
    }
    any ADD(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "double" || a.ptr->type == "int")
        {
            double val = *(double *)(this->val) + *((double *)a.ptr->val);
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any SUB(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "double" || a.ptr->type == "int")
        {
            double val = *(double *)(this->val) - *((double *)a.ptr->val);
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("SUB does not support: (" + type + "," + a.ptr->type + ")");
    }
    any MUL(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "double" || a.ptr->type == "int")
        {
            double val = *(double *)(this->val) * (*((double *)a.ptr->val));
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("MUL does not support: (" + type + "," + a.ptr->type + ")");
    }
    any DIV(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "double" || a.ptr->type == "int")
        {
            if ((*(double *)a.ptr->val) == 0)
            {
                throw runtime_error("Division by zero");
            }
            double val = *(double *)(this->val) / *((double *)a.ptr->val);
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("DIV does not support: (" + type + "," + a.ptr->type + ")");
    }
    any POW(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "double" || a.ptr->type == "int")
        {
            double val = pow(*(double *)(this->val), *((double *)a.ptr->val));
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("POW does not support: (" + type + "," + a.ptr->type + ")");
    }
    ~ptr_double()
    {
        delete (double *)val;
    }
};

class ptr_int : public ptr_gen
{
public:
    ptr_int(int val)
    {
        type = "int";
        this->val = new int(val);
    }
    ptr_int *deepCopy()
    {
        return new ptr_int(*(int *)val);
    }
    any ADD(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "int")
        {
            int val = *(int *)(this->val) + *((int *)a.ptr->val);
            st.outfunc();
            return any(new ptr_int(val));
        }
        if (a.ptr->type == "double")
        {
            double val = *(double *)(this->val) + *((double *)a.ptr->val);
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any SUB(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "int")
        {
            int val = *(int *)(this->val) - *((int *)a.ptr->val);
            st.outfunc();
            return any(new ptr_int(val));
        }
        if (a.ptr->type == "double")
        {
            double val = *(double *)(this->val) - *((double *)a.ptr->val);
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("SUB does not support: (" + type + "," + a.ptr->type + ")");
    }
    any MUL(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "int")
        {
            int val = *(int *)(this->val) * (*((int *)a.ptr->val));
            st.outfunc();
            return any(new ptr_int(val));
        }
        if (a.ptr->type == "double")
        {
            double val = *(double *)(this->val) * (*((double *)a.ptr->val));
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("MUL does not support: (" + type + "," + a.ptr->type + ")");
    }
    any DIV(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "double" || a.ptr->type == "int")
        {
            if ((*(double *)a.ptr->val) == 0)
            {
                throw runtime_error("Division by zero");
            }
            double val = *(double *)(this->val) / *((double *)a.ptr->val);
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("DIV does not support: (" + type + "," + a.ptr->type + ")");
    }
    any MOD(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "int")
        {
            int val = *(int *)(this->val) % (*((int *)a.ptr->val));
            st.outfunc();
            return any(new ptr_int(val));
        }
        throw runtime_error("MOD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any POW(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "int")
        {
            int val = pow(*(int *)(this->val), (*((int *)a.ptr->val)));
            st.outfunc();
            return any(new ptr_int(val));
        }
        if (a.ptr->type == "double")
        {
            double val = pow(*(double *)(this->val), (*((double *)a.ptr->val)));
            st.outfunc();
            return any(new ptr_double(val));
        }
        throw runtime_error("POW does not support: (" + type + "," + a.ptr->type + ")");
    }
    ~ptr_int()
    {
        delete (int *)val;
    }
};

class ptr_array : public ptr_gen
{
public:
    ptr_array(const vector<any> &val)
    {
        type = "array";
        this->val = new vector<any>(val);
    }
    ptr_array *deepCopy()
    {
        return new ptr_array(*(vector<any> *)val);
    }
    any &at(const any &i, const pos &p)
    {
        st.infunc(p);
        vector<any> *arr = (vector<any> *)val;
        if (i.ptr->type != "int")
        {
            throw runtime_error("index should be an integer (given index was " + i.ptr->type + ")");
        }
        int idx = *(int *)i.ptr->val;
        if (idx >= arr->size())
        {
            throw runtime_error("out of index");
        }
        st.outfunc();
        return (*arr)[idx];
    }
    void add_item(const any &item, const pos &p)
    {
        st.infunc(p);
        vector<any> *arr = (vector<any> *)val;
        arr->push_back(item);
        st.outfunc();
    }
    int size(const pos &p)
    {
        st.infunc(p);
        vector<any> *ptr = (vector<any> *)val;
        int sz = ptr->size();
        st.outfunc();
        return sz;
        // return any(new ptr_int(sz));
    }

    any CONCAT(const any &a, const pos &p)
    {
        st.infunc(p);
        if (a.ptr->type == "array")
        {
            vector<any> *arr1 = (vector<any> *)val;
            vector<any> *arr2 = (vector<any> *)a.ptr->val;
            vector<any> arr(*arr1);
            for (auto item : *arr2)
            {
                arr.push_back(item);
            }
            st.outfunc();
            return any(new ptr_array(arr));
        }
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    ~ptr_array()
    {
        delete (vector<any> *)val;
    }
};

// defining "any" function
inline any::any(const any &a)
{
    this->ptr = a.ptr->deepCopy();
}
inline void any::operator=(const any &a)
{
    delete ptr;
    ptr = a.ptr->deepCopy();
}
inline any::~any()
{
    delete ptr;
}

// defining outstream
ostream &operator<<(ostream &o, const any &a)
{
    if (a.ptr->type == "string")
    {
        string *str = (string *)a.ptr->val;
        o << *str;
        return o;
    }
    if (a.ptr->type == "int")
    {
        int val = *(int *)a.ptr->val;
        o << to_string(val);
        return o;
    }
    if (a.ptr->type == "double")
    {
        double *val = (double *)a.ptr->val;
        o << to_string(*val);
        return o;
    }
    if (a.ptr->type == "none")
    {
        o << string("none");
        return o;
    }
    throw runtime_error("print operator does not support: (" + a.ptr->type + ")");
}

#endif