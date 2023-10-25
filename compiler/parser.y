%skeleton "lalr1.cc"
%require "3.8.1"
%header 

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%language "C++" 

%code requires{
#include <iostream>
class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
# include "driver.hh"
}

%define api.token.prefix {TOK_}

%token <int> DIGIT "digit"
%nterm <int> program

%%
%start S;

S: 
    program {
        drv.res = $1 ;
    }
    ;
program:
    DIGIT program {
        $$ = $1 + $2 ;
    }
    |
    DIGIT {
        $$ = $1 ;
    }
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}