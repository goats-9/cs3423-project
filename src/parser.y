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
    LET "let_token"
    CONST "const_token"
    STRUCT "struct_token"
    IF "if_token"
    ELSE "else_token"
    VOID "void_token"
    WHILE "while_token"
    FUN "function_token"
    RETURN "return_token"
    RETURNS "returns_token"
    BREAK "break_token"
    CONTINUE "continue_token"

// punctuators
%token  
    EQUAL "equal_token" 
    COLON "colon_token" 
    SEMICOLON "semcolon_token"
    DOT "dot_token" 
    COMMA "comma_token"
    OPEN_SQUARE_BRAC "open_square_bracket_token" 
    CLOSE_SQUARE_BRAC "close_square_bracket_token"
    OPEN_CURLY "open_curly_bracket_token"
    CLOSE_CURLY "close_curly_bracket_token"
    OPEN_PARENTHESIS "open_parenthesis_token"
    CLOSE_PARENTHESIS "close_parenthesis_token"

// identifiers
%token
    <std::string> ID "identifier_token"

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

variable_list: ID
             | ID COMMA variable_list
             ;
declaration: LET variable_list
           | CONST variable_list
           ;
declaration_stmt: declaration SEMICOLON
                | declaration EQUAL expression_list SEMICOLON
                | declaration EQUAL array_initializer SEMICOLON
                | declaration EQUAL ID OPEN_PARENTHESIS array_initializer COMMA ID CLOSE_PARENTHESIS SEMICOLON
                | struct_declaration
                ;
array_initializer: OPEN_SQUARE_BRAC expression_list CLOSE_SQUARE_BRAC ;
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
table_expression: OPEN_SQUARE_BRAC INT CLOSE_SQUARE_BRAC
                | OPEN_SQUARE_BRAC RANGE CLOSE_SQUARE_BRAC
                ;
expression_list: expression
               | expression COMMA expression_list
               ;
expression_stmt: variable_list EQUAL expression_list SEMICOLON ;

statement: expression_stmt
         | declaration_stmt
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