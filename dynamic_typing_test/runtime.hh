#ifndef RUNTIME_HH
#define RUNTIME_HH

#define ACCESS_CODE                                 \
    if (!(p->mem[id].ptr))                          \
    {                                               \
        p->mem[id].ptr = (ptr_gen *)new ptr_none(); \
    }                                               \
    return p->mem[id];
#define RUN_CODE                                                                \
    any (myclass::*func_ptr)(vector<any>);                                      \
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
using namespace std;

class any;

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
    virtual any &access(const string &id) = 0;
    // running a function with name id
    virtual any run(const string &id, const vector<any> &params) = 0;
    // accessing i indexed element of array
    virtual any &operator[](int i) = 0;
    // ADD
    virtual any ADD(const any &a) = 0;
    // CONCAT
    virtual any CONCAT(const any &a) = 0;
    // add item
    virtual void add_item(const any &item) = 0;
    // size
    virtual any size() = 0;
    // destructor
    virtual ~ptr_gen(){};
};

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
    any(const any &a)
    {
        this->ptr = a.ptr->deepCopy();
    }
    // accesing any function of gen
    ptr_gen *operator->()
    {
        return ptr;
    }
    // accessing i indexed element of array
    any &operator[](int i)
    {
        return (*ptr)[i];
    }
    // assignment
    void operator=(const any &a)
    {
        delete ptr;
        ptr = a.ptr->deepCopy();
    }
    // destructor
    ~any()
    {
        delete ptr;
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
    any &access(const string &id)
    {
        throw runtime_error("invalid access");
    }
    any run(const string &id, const vector<any> &params)
    {
        throw runtime_error("invalid access");
    }
    any &operator[](int i)
    {
        throw runtime_error("Not an array");
    }
    any ADD(const any &a)
    {
        if (a.ptr->type == "int")
        {
            int val = *(int *)(this->val) + *((int *)a.ptr->val);
            return any(new ptr_int(val));
        }
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any CONCAT(const any &a)
    {
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        throw runtime_error("Not an array or string");
    }
    ~ptr_int()
    {
        delete (int *)val;
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
    any &access(const string &id)
    {
        throw runtime_error("invalid access");
    }
    any run(const string &id, const vector<any> &params)
    {
        throw runtime_error("invalid access");
    }
    any &operator[](int i)
    {
        throw runtime_error("Not an array");
    }
    any ADD(const any &a)
    {
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any CONCAT(const any &a)
    {
        if (a.ptr->type == "string")
        {
            string val = *(string *)(this->val) + *((string *)a.ptr->val);
            return any(new ptr_string(val));
        }
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        string *ptr = (string *)val;
        int sz = ptr->size();
        return any(new ptr_int(sz));
    }
    ~ptr_string()
    {
        delete (string *)val;
    }
};

class ptr_none : ptr_gen
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
    any &access(const string &id)
    {
        throw runtime_error("invalid access");
    }
    any run(const string &id, const vector<any> &params)
    {
        throw runtime_error("invalid access");
    }
    any &operator[](int i)
    {
        throw runtime_error("Not an array");
    }
    any ADD(const any &a)
    {
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any CONCAT(const any &a)
    {
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        throw runtime_error("Not an array or string");
    }
    ptr_gen *operator+(ptr_gen *a)
    {
        throw runtime_error("+ operator does not support: (" + type + "," + a->type + ")");
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
    any &access(const string &id)
    {
        throw runtime_error("invalid access");
    }
    any run(const string &id, const vector<any> &params)
    {
        throw runtime_error("invalid access");
    }
    any &operator[](int i)
    {
        vector<any> *arr = (vector<any> *)val;
        if (i >= arr->size())
        {
            throw runtime_error("out of index");
        }
        return (*arr)[i];
    }
    any ADD(const any &a)
    {
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any CONCAT(const any &a)
    {
        if (a.ptr->type == "array")
        {
            vector<any> *arr1 = (vector<any> *)val;
            vector<any> *arr2 = (vector<any> *)a.ptr->val;
            vector<any> arr(*arr1);
            for(auto item: *arr2)
            {
                arr.push_back(item);
            }
            return any(new ptr_array(arr));
        }
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    void add_item(const any &item)
    {
        vector<any> *arr = (vector<any> *)val;
        arr->push_back(item);
    }
    any size()
    {
        vector<any> *ptr = (vector<any> *)val;
        int sz = ptr->size();
        return any(new ptr_int(sz));
    }
    ~ptr_array()
    {
        delete (vector<any> *)val;
    }
};

class ptr_custom : public ptr_gen
{
public:
    any &operator[](int i)
    {
        throw runtime_error("Not an array");
    }
    any ADD(const any &a)
    {
        throw runtime_error("ADD does not support: (" + type + "," + a.ptr->type + ")");
    }
    any CONCAT(const any &a)
    {
        throw runtime_error("CONCAT does not support: (" + type + "," + a.ptr->type + ")");
    }
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        throw runtime_error("Not an array or string");
    }
};

ostream &operator<<(ostream &o, const any &a)
{
    if (a.ptr->type == "string")
    {
        string str = *(string *)a.ptr->val;
        o << str;
        return o;
    }
    if (a.ptr->type == "int")
    {
        int val = *(int *)a.ptr->val;
        o << to_string(val);
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