#ifndef STATE_HH
#define STATE_HH

#include "location.hh"
#include <stack>
using namespace std;

class state
{
public:
    stack<yy::location> loc;
    stack<string> func_call;
    string file;

    state(const string &file_name)
    {
        this->file = file_name;
    }
    void infunc(int start_line, int start_col, int end_line, int end_col, const string &function_call)
    {
        loc.push(
            yy::location(
                yy::position(&file,start_line,start_col)
                ,
                yy::position(&file,end_line,end_col)
            )
        );
        func_call.push(function_call);
    }
    void outfunc()
    {
        loc.pop();
        func_call.pop();
    }
};

#endif