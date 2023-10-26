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
using namespace std;
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

%token STRUCT "struct" EQUAL "=" COLON ":" DOT "." OPEN_SQUARE_BRAC "[" CLOSE_SQUARE_BRAC "]"
%token
    <string> DATATYPE "datatype" 
    <string> ID "identifier"
    <int> DIGIT "digit"
    <string> ARITHMETIC "arithmetic"
    <string> BIOP "binary operator"
    <string> UNIOP "unary operator"

%nterm <int> program

%%
// start symbol
%start S;

// Write grammar rules below

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