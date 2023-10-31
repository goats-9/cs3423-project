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

%%
%start program;
program: /* empty */
        | program program_element
        ; 
program_element: function_definition
              | struct_declaration
              ;

constant: INT
        | STRING
        | BOOL
        | DOUBLE
        | DATE
        | TIME
        | RANGE
        ;

declaration: LET
           | CONST
           ;
declaration_stmt: declaration variable_list SEMICOLON 
                | declaration assignment_stmt
                ;
assignment_stmt: ID EQUAL assigns_to SEMICOLON
               | ID EQUAL assigns_to COMMA assignment_stmt
               ;
assigns_to: expression
          | array_initializer
          | ID OPEN_PARENTHESIS array_initializer COMMA ID CLOSE_PARENTHESIS
          ;

array_initializer: OPEN_SQUARE_BRAC variable_list CLOSE_SQUARE_BRAC ;
struct_declaration: STRUCT ID OPEN_CURLY struct_member_list CLOSE_CURLY SEMICOLON ;
struct_member_list: /* empty */
                  | struct_member_list declaration_stmt
                  | struct_member_list function_definition
                  ;

expression: constant
            | ID
            | UNIOP expression
            | expression BIOP expression
            | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
            | ID OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS
            | ID DOT ID
            | ID table_expression
            | ID table_expression table_expression
            ;
expression_list: expression
               | expression COMMA expression_list
               ;
table_expression: OPEN_SQUARE_BRAC INT CLOSE_SQUARE_BRAC
                | OPEN_SQUARE_BRAC RANGE CLOSE_SQUARE_BRAC
                ;

statement: declaration_stmt
         | assignment_stmt
         | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
         | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement ELSE compound_statement
         | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
         | function_definition
         | BREAK SEMICOLON
         | CONTINUE SEMICOLON
         ;
return_stmt: RETURN expression SEMICOLON
           | RETURN SEMICOLON
           ;
statement_list: /* empty */
              | statement_list statement
              ; 
compound_statement: OPEN_CURLY statement_list CLOSE_CURLY ;

variable_list: ID
             | ID COMMA variable_list
             ;
parameter_list: /* empty */
              | variable_list
              ;
function_definition: FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS function_body ;
function_body: OPEN_CURLY statement_list return_stmt CLOSE_CURLY ;
%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}