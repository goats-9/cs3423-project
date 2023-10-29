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
}

// The parsing context
%param { tabulate::driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "tabulate.hh"
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

// identifiers
%token
    <std::string> ID "identifier"

// operators
%token
    <std::string> BIOP "binary operator"
    <std::string> UNIOP "unary operator"

// constant
%token
    <int> INT "integer"
    <std::string> STRING "string"
    <bool> BOOL "boolean"
    <double> DOUBLE "double"
    <tabulate::date> DATE "date"
    <tabulate::time> TIME "time"

%nterm <int> program

%%
// start symbol
%start S;

// Write grammar rules below

S: 
    program {
        drv.result = $1 ;
    }
    ;
program:
    INT program {
        $$ = $1 + $2 ;
    }
    |
    INT {
        $$ = $1 ;
    }
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}