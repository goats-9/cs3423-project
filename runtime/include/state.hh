#pragma once

#include "location.hh"
#include <stack>
#include <string>
#include <iostream>

class pos
{
public:
    int s_line,s_col,e_line,e_col;
    std::string func_call;
    
    pos(int start_line, int start_col, int end_line, int end_col, const std::string &function_call)
    {
        s_line = start_line;
        s_col = start_col;
        e_line = end_line;
        e_col = end_col;
        func_call = function_call;
    }
};

class state
{
public:
    std::stack<yy::location> loc;
    std::stack<std::string> func_call;
    std::string file;
    state (){}
    state(const std::string &file_name)
    {
        this->file = file_name;
    }
    void infunc(const pos &p)
    {
        loc.push(
            yy::location(
                yy::position(&file,p.s_line,p.s_col)
                ,
                yy::position(&file,p.e_line,p.e_col)
            )
        );
        func_call.push(p.func_call);
    }
    void outfunc()
    {
        loc.pop();
        func_call.pop();
    }
};

std::ostream& operator<<(std::ostream& o, state s);