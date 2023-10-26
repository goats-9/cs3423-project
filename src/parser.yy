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

// The parsing context
%param { driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
# include "driver.hh"
}


// reserved keywords
%token
    CLASS "class"
    IF "if"
    ELSE "else"
    VOID "void"
    WHILE "while"
    FUN "fun"
    RETURN "return"
    RETURNS "returns"
    BREAK "break"
    CONTINUE "continue"
    MAIN "main"

// punctuators
%token  
    EQUAL "=" 
    COLON ":" 
    SEMICOLON ";"
    DOT "." 
    COMMA ","
    OPEN_SQUARE_BRAC "[" 
    CLOSE_SQUARE_BRAC "]"
    OPEN_CURLY "{"
    CLOSE_CURLY "}"
    OPEN_PARENTHESIS "("
    CLOSE_PARENTHESIS ")"

// datatypes
%token
    <string> DATATYPE "datatype" 

// identifiers
%token
    <string> ID "identifier"

// operators
%token
    <string> ARITHMETIC "arithmetic"
    <string> BIOP "binary operator"
    <string> UNIOP "unary operator"

// constant
%token
    <int> DIGIT "digit"

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