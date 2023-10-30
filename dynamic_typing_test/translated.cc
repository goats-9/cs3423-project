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
        return params[0]->ADD(params[1]);
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

// class_ptr
class ptr_myclass : public ptr_custom
{
public:
    ptr_myclass(const myclass &val)
    {
        type = "myclass";
        this->val = new myclass(val);
    }
    ptr_myclass *deepCopy()
    {
        return new ptr_myclass(*(myclass *)val);
    }
    any &access(const string &id)
    {
        myclass *p = (myclass *)(this->val);
        ACCESS_CODE
    }
    any run(const string &id, const vector<any> &params)
    {
        myclass *p = (myclass *)(this->val);
        RUN_CODE
    }
    ~ptr_myclass()
    {
        delete (myclass *)val;
    }
};

int main()
{
    state st("test.tlbt");
    try
    {
        any a(new ptr_int(67));
        any b(new ptr_string("90"));
        // st.infunc();
        cout << a->ADD(a) << "\n";
        cout << b->ADD(b) << "\n";
        any c(new ptr_myclass(myclass()));
        c->access("at1") = any(new ptr_int(1));
        cout << c->run("add", {a, c->access("at1"), a}) << "\n";
        any arr(new ptr_array({any(new ptr_int(23)),
                               any(new ptr_array({any(new ptr_string("hello")),
                                                  any(new ptr_string("world"))}))}));
        cout << arr[1][0]->CONCAT(arr[1][1]) << "\n";
        arr->add_item(any(new ptr_int(9)));
        cout << arr[2] << "\n";
    }
    catch (const runtime_error &e)
    {
        cerr << st.loc.top() << ": runtime error, " << e.what() << "\n";
        while (!st.loc.empty())
        {
            cerr << "\tFrom " << st.loc.top() << ": " << st.func_call.top() << '\n';
            st.func_call.pop();
            st.loc.pop();
        }
        return 1;
    }

    return 0;
}