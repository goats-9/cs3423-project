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
namespace tabulate
{
    class driver;
}
#include "tabulate/types.hh"
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
    LET "let"
    CONST "const"
    STRUCT "struct"
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
%left
    <std::string> UNIOP "unary operator"
%right
    <std::string> BIOP "binary operator"

// constant
%token
    <int> INT "integer"
    <std::string> STRING "string"
    <bool> BOOL "boolean"
    <double> DOUBLE "double"
    <tabulate::date> DATE "date"
    <tabulate::time> TIME "time"
    <std::string> RANGE "range"
%nterm <int> program

%%
%start S;

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

//type constants:
constant: INT | STRING | BOOL | DOUBLE | DATE | TIME | RANGE;

//declaration & assignment:
declaration: LET variable_list
           | LET variable_list EQUAL expression_list
           | CONST variable_list
           | CONST variable_list EQUAL expression_list
           ;

declaration_stmt: declaration SEMICOLON
                | declaration EQUAL array_initializer SEMICOLON // array
                | declaration EQUAL ID OPEN_PARENTHESIS array_initializer COMMA ID CLOSE_PARENTHESIS SEMICOLON // formula
                ;

struct_declaration: STRUCT ID EQUAL OPEN_CURLY struct_member_list CLOSE_CURLY SEMICOLON ;
struct_member_list: /* empty */
                  | struct_member_list declaration_stmt SEMICOLON ;
                  | struct_member_list function_definition SEMICOLON ;

//array
array_initializer: OPEN_SQUARE_BRAC expression_list CLOSE_SQUARE_BRAC;

// table declaration below:

//expressions & operators:
expression:
    constant
    | ID
    | UNIOP expression
    | expression BIOP expression
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
    | ID DOT ID // member access expression
    ;

expression_list: expression
               | expression COMMA expression_list
               ;

variable_list: ID
             | ID COMMA variable_list ;

expression_stmt: variable_list EQUAL expression SEMICOLON ;

//general statements
statement: expression_stmt
         | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
         | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement ELSE compound_statement
         | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
         | BREAK SEMICOLON
         | CONTINUE SEMICOLON
         | RETURN expression SEMICOLON
         | OPEN_CURLY program CLOSE_CURLY
         ;

statement_list: /* empty */
              | statement_list statement SEMICOLON
              ; 
compound_statement: OPEN_CURLY statement_list CLOSE_CURLY ;

//function declarations
function_definition: FUN ID OPEN_PARENTHESIS variable_list CLOSE_PARENTHESIS compound_statement ;
// function call just same as declarations
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}