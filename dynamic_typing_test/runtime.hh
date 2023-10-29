#ifndef RUNTIME_HH
#define RUNTIME_HH

#define ACCESS_CODE                             \
    if (!(p->mem[id].ptr))                      \
    {                                           \
        p->mem[id].ptr = (gen *)new none_ptr(); \
    }                                           \
    return p->mem[id];
#define RUN_CODE                                    \
    any (myclass::*func_ptr)(vector<any>);          \
    func_ptr = p->func[id];                         \
    if (func_ptr && p->params[id] == params.size()) \
    {                                               \
        return (p->*func_ptr)(params);              \
    }                                               \
    if (!(func_ptr))                                \
    {                                               \
        throw runtime_error("function not found");                \
    }                                               \
    throw runtime_error("parameters does not match");

#include <string>
#include <stdexcept>
#include <map>
#include <vector>
using namespace std;

class any;

class gen
{
public:
    // type of variable
    string type;
    // data stored
    void *val;

    // gets deep copy
    virtual gen *deepCopy() = 0;
    // accessing a member
    virtual any &access(const string &id) = 0;
    // running a function with name id
    virtual any run(const string &id,const vector<any> &params) = 0;
    // accessing i indexed element of array
    virtual any &operator[](int i) = 0;
    // add operator
    virtual gen *operator+(gen *a) = 0;
    // add item 
    virtual void add_item(const any &item) = 0;
    // size
    virtual any size() = 0;
    // destructor
    virtual ~gen(){};
};

// smart pointer
class any
{
public:
    // pointer to general type
    gen *ptr;

    // default contructor
    any()
    {
        ptr = NULL;
    }
    // contructor with pointer
    any(gen *ptr)
    {
        this->ptr = ptr;
    }
    // copy constructor
    any(const any &a)
    {
        this->ptr = a.ptr->deepCopy();
    }
    // accesing any function of gen
    gen *operator->()
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
    // add
    any operator+(const any &b)
    {
        gen *res = *(this->ptr) + b.ptr;
        return any(res);
    }
    // destructor
    ~any()
    {
        delete ptr;
    }
};

class int_ptr : public gen
{
public:
    int_ptr(int val)
    {
        type = "int";
        this->val = new int(val);
    }
    int_ptr *deepCopy()
    {
        return new int_ptr(*(int *)val);
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
    gen *operator+(gen *a)
    {
        if (a->type == "int")
        {
            int val = *(int *)(this->val) + *((int *)a->val);
            return new int_ptr(val);
        }
        throw runtime_error("+ operator does not support: (" + type + "," + a->type + ")");
    }
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        throw runtime_error("Not an array or string");
    }
    ~int_ptr()
    {
        delete (int *)val;
    }
};

class string_ptr : public gen
{
public:
    string_ptr(const string &val)
    {
        type = "string";
        this->val = new string(val);
    }
    string_ptr *deepCopy()
    {
        return new string_ptr(*(string *)val);
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
    gen *operator+(gen *a)
    {
        if (a->type == "string")
        {
            string val = *(string *)(this->val) + *((string *)a->val);
            return new string_ptr(val);
        }
        throw runtime_error("+ operator does not support: (" + type + "," + a->type + ")");
    }
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        string *ptr = (string *) val;
        int sz = ptr->size();
        return any(new int_ptr(sz));
    }
    ~string_ptr()
    {
        delete (string *)val;
    }
};

class none_ptr : gen
{
public:
    none_ptr()
    {
        type = "none";
        this->val = NULL;
    }
    none_ptr *deepCopy()
    {
        return new none_ptr();
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
    void add_item(const any &item)
    {
        throw runtime_error("Not an array");
    }
    any size()
    {
        throw runtime_error("Not an array or string");
    }
    gen *operator+(gen *a)
    {
        throw runtime_error("+ operator does not support: (" + type + "," + a->type + ")");
    }
};

class array_ptr : public gen
{
public:
    array_ptr(const vector<any> &val)
    {
        type = "array";
        this->val = new vector<any>(val);
    }
    array_ptr *deepCopy()
    {
        return new array_ptr(*(vector<any> *)val);
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
    gen *operator+(gen *a)
    {
        throw runtime_error("+ operator does not support: (" + type + "," + a->type + ")");
    }
    void add_item(const any &item)
    {
        vector<any> *arr = (vector<any> *)val;
        arr->push_back(item);
    }
    any size()
    {
        vector<any> *ptr = (vector<any> *) val;
        int sz = ptr->size();
        return any(new int_ptr(sz));
    }
    ~array_ptr()
    {
        delete (vector<any> *)val;
    }
};

class custom_ptr : public gen
{
public:
    any &operator[](int i)
    {
        throw runtime_error("Not an array");
    }
    gen *operator+(gen *a)
    {
        throw runtime_error("+ operator does not support: (" + type + "," + a->type + ")");
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