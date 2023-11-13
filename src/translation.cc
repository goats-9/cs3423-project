#include "translation.hh"

std::string tabulate::translatePos(yy::location l,const std::string &func_call)
{
    return "pos(" + std::to_string(l.begin.line) + "," + std::to_string(l.begin.column) + "," + std::to_string(l.end.line) + "," + std::to_string(l.end.column) + ",\"" + func_call + "\")";
}