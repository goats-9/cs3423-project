#pragma once

#include <string>
#include <vector>
#include "symtab.hh"

namespace yy
{
    std::string &operator<< (std::string&s,std::string a);
    std::string &operator<< (std::string&s,const char* a);
}
namespace tabulate {
    /* types of non terminal starts */
    typedef struct vector_of_string
    {
        std::vector<std::string> sem;
        std::string trans;
    } vector_of_string;
    typedef struct struct_member_list
    {
        std::vector<int> constr_args_list;
        std::vector<tabulate::func_symtrec> func_in_struct;
        std::string trans;
    } struct_member_list;
    typedef struct Int
    {
        int sem;
        std::string trans;
    }Int;
    typedef struct String
    {
        std::string sem;
        std::string trans;
    } String;
    typedef struct constant
    {
        std::string type;
        std::string value;
    } constant;
    typedef struct instance
    {
        std::string exp;
        std::string attribute;
    } instance;
    typedef struct Default
    {
        std::string trans;
    } Default;
    /* types of non terminal ends */
}
