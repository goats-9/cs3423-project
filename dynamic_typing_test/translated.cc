#include "runtime.hh"
#include <iostream>
using namespace std;

class yy_myclass
{
public:
    // members
    map<string, any> mem;
    // func params number
    map<string, int> params;
    // function
    map<string, any (yy_myclass::*)(vector<any>)> func;
    // function declaration (as given in user input file)
    any add(vector<any> params)
    {
        return params[0]->ADD(params[1]);
    }
    // put all functions in a map
    class yy_inner
    {
    public:
        // members
        map<string, any> mem;
        // func params number
        map<string, int> params;
        // function
        map<string, any (yy_inner::*)(vector<any>)> func;
        // function declaration (as given in user input file)
        any twice(vector<any> params)
        {
            return params[0]->ADD(params[0]);
        }
        void func_decl()
        {
            func["twice"] = &yy_inner::twice;
            params["twice"] = 1;
        }
        yy_inner()
        {
            func_decl();
        }
    };
    class ptr_inner : public ptr_gen
    {
    public:
        ptr_inner(const yy_inner &val)
        {
            type = "myclass::inner";
            this->val = new yy_inner(val);
        }
        ptr_inner *deepCopy()
        {
            return new ptr_inner(*(yy_inner *)val);
        }
        any &access(const string &id)
        {
            yy_inner *p = (yy_inner *)(this->val);
            ACCESS_CODE
        }
        any run(const string &id, const vector<any> &params)
        {
            yy_inner *p = (yy_inner *)(this->val);
            any (yy_inner::*func_ptr)(vector<any>);
            RUN_CODE
        }
        ~ptr_inner()
        {
            delete (yy_myclass *)val;
        }
    };
    void func_decl()
    {
        func["add"] = &yy_myclass::add;
        params["add"] = 2;
    }
    void mem_decl()
    {
        mem["a"] = any(new ptr_inner(yy_inner()));
    }
    yy_myclass()
    {
        mem_decl();
        func_decl();
    }
};

// class_ptr
class ptr_myclass : public ptr_gen
{
public:
    ptr_myclass(const yy_myclass &val)
    {
        type = "myclass";
        this->val = new yy_myclass(val);
    }
    ptr_myclass *deepCopy()
    {
        return new ptr_myclass(*(yy_myclass *)val);
    }
    any &access(const string &id)
    {
        yy_myclass *p = (yy_myclass *)(this->val);
        ACCESS_CODE
    }
    any run(const string &id, const vector<any> &params)
    {
        yy_myclass *p = (yy_myclass *)(this->val);
        any (yy_myclass::*func_ptr)(vector<any>);
        RUN_CODE
    }
    ~ptr_myclass()
    {
        delete (yy_myclass *)val;
    }
};

int main()
{
    state st("test.tlbt");
    any temp;
    try
    {
        // a = 67;
        any a(new ptr_int(67));
        // b = "90";
        any b(new ptr_string("90"));
        // DISP(DIV(a,0));
        st.infunc(18, 7, 18, 14, "DIV(a,0)");
        temp = a->DIV(any(new ptr_int(2)));
        st.outfunc();
        st.infunc(18, 1, 18, 15, "print(DIV(a,0))");
        cout << temp << "\n";
        st.outfunc();
        temp = a->ADD(a);
        // st.outfunc();
        // st.infunc(18,1,18,16,"print(ADD(a,a))");
        cout << temp << "\n";
        // st.outfunc();
        cout << b->CONCAT(b) << "\n";
        any c(new ptr_myclass(yy_myclass()));
        c->access("at1") = any(new ptr_int(1));
        cout << c->access("a") << "\n";
        cout << c->run("add", {a, c->access("at1")}) << "\n";
        any arr(new ptr_array({any(new ptr_int(23)),
                               any(new ptr_array({any(new ptr_string("hello")),
                                                  any(new ptr_string("world"))}))}));
        cout << arr[1][0]->CONCAT(arr[1][1]) << "\n";
        arr->add_item(any(new ptr_int(9)));
        cout << arr[2] << "\n";
        any sz = any(new ptr_int(arr->size()));
        cout << sz << "\n";
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
        // cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}