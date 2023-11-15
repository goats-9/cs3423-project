#pragma once

#include <string>
#include <vector>
#include "symtab.hh"

namespace yy
{
    std::string &operator<< (std::string&s,std::string a);
    std::string &operator<< (std::string&s,const char* a);
}
#define TABULATE_STRUCT 0
#define TABULATE_FUNC 1

namespace tabulate {
    /* types of non terminal starts */
    typedef struct function_info
    {
        std::string name;
        int params;
    }function_info;
    typedef struct struct_info
    {
        std::string name;
    }struct_info;
    typedef struct vector_of_string
    {
        std::vector<std::string> sem;
        std::string trans;
    } vector_of_string;
    typedef struct struct_member_list
    {
        std::vector<int> constr_args_list;
        std::vector<function_info> func_in_struct;
        std::string trans;
    } struct_member_list;
    typedef struct program_element
    {
        int type;
        struct_info structInfo;
    }program_element;
    typedef struct program
    {
        std::vector<struct_info> structList;
    }program;
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
