#include "runtime.hh"
#include <iostream>
using namespace std;

class myclass
{
public:
    // members
    map<string, any> mem;
    // func params number
    map<string, int> params;
    // function
    map<string, any (myclass::*)(vector<any>)> func;
    // function declaration (as given in user input file)
    any add(vector<any> params)
    {
        return params[0] + params[1];
    }
    // put all functions in a map
    void func_decl()
    {
        func["add"] = &myclass::add;
        params["add"] = 2;
    }
    myclass()
    {
        func_decl();
    }
};

class myclass_ptr : public custom_ptr
{
public:
    myclass_ptr(myclass val)
    {
        type = "myclass";
        this->val = new myclass(val);
    }
    myclass_ptr *deepCopy()
    {
        return new myclass_ptr(*(myclass *)val);
    }
    any &access(string id)
    {
        myclass *p = (myclass *)(this->val);
        ACCESS_CODE
    }
    any run(string id, vector<any> params)
    {
        myclass *p = (myclass *)(this->val);
        RUN_CODE
    }
    ~myclass_ptr()
    {
        delete (myclass *)val;
    }
};

int main()
{
    try
    {
        any a(new int_ptr(67));
        any b(new string_ptr("90"));

        cout << a + a << "\n";
        cout << b + b << "\n";
        any c(new myclass_ptr(myclass()));
        c->access("at1") = any(new int_ptr(1));
        cout << c->run("add", {a, c->access("at1")}) << "\n";

        any arr(new array_ptr({
            any(new int_ptr(23))
            , 
            any(new array_ptr({
                any(new string_ptr("hello"))
                , 
                any(new string_ptr("world"))
            }))
        }));

        cout << arr[1][0] + arr[1][1] << "\n";
    }
    catch (const runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}