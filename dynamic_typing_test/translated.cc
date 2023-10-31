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
        return params[0]->ADD(params[1],pos(1,1,1,1,"ADD(a,b)"));
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
            return params[0]->ADD(params[0],pos(1,1,1,1,"ADD(a,a)"));
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
    any &access(const string &id, const pos &ps)
    {
        st.infunc(ps);
        yy_myclass *p = (yy_myclass *)(this->val);
        ACCESS_CODE
    }
    any run(const string &id, const vector<any> &params,const pos &ps)
    {
        st.infunc(ps);
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
    st = state("test.tlbt");
    any temp;
    try
    {
        // let a = 67;
        any a(new ptr_int(67));
        // let b = "90";
        any b(new ptr_string("90"));
        // DISP(DIV(a,0));
        cout << a->DIV(any(new ptr_int(2)),pos(18,7,18,14,"DIV(a,0)")) << "\n";
        // DISP(ADD(a,a));
        cout << a->ADD(a,pos(18,6,18,13,"ADD(a,a)")) << "\n";
        // DISP(CONCAT(b,b));
        cout << b->CONCAT(b,pos(18,5,18,15,"CONCAT(b,b)")) << "\n";
        // let c = myclass();
        any c(new ptr_myclass(yy_myclass()));
        // c.at1 = 1;
        c->access("at1",pos(1,1,1,1,"c.at1")) = any(new ptr_int(1));
        // DISP(c.at1);
        cout << c->access("at1",pos(1,1,1,1,"c.at1")) << "\n";
        cout << c->run("add", {a, c->access("at1",pos(1,1,1,1,"c.at1"))},pos(1,1,1,1,"c.add(a,c.at1)")) << "\n";
        any arr(new ptr_array({any(new ptr_int(23)),
                               any(new ptr_array({any(new ptr_string("hello")),
                                                  any(new ptr_string("world"))}))}));
        cout << arr->at(any(new ptr_int(1)),pos(1,1,1,1,"arr[1]"))->at(any(new ptr_int(0)),pos(1,1,1,1,"arr[1][0]"))->CONCAT(arr->at(any(new ptr_int(1)),pos(1,1,1,1,"arr[1]"))->at(any(new ptr_int(1)),pos(1,1,1,1,"arr[1][1]")),pos(1,1,1,1,"CONCAT(arr[1][0],arr[1][1])")) << "\n";
        arr->add_item(any(new ptr_int(9)),pos(1,1,1,1,"arr.add_item(9)"));
        cout << arr->at(any(new ptr_int(2)),pos(1,1,1,1,"arr[2]")) << "\n";
        any sz = any(new ptr_int(arr->size(pos(1,1,1,1,"arr.size()"))));
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