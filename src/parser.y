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
#include "../include/types.hh"
}

// The parsing context
%param { tabulate::driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "../include/tabulate.hh"
}

// Reserved keywords
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

// Punctuators
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

// Identifiers
%token 
    <std::string> ID "identifier_token"

// Operators
%left
    <std::string> UNIOP "unary operator"
%right
    <std::string> BIOP "binary operator"

// Constants
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

declare: LET
       | CONST
       ;
declaration_stmt: declare variable_list SEMICOLON 
                | declare assignment_stmt
                ;
assignment_stmt: variable EQUAL assignment_target SEMICOLON
               | variable EQUAL assignment_target COMMA assignment_stmt
               ;
variable: ID
        | ID table_expression
        | ID table_expression table_expression
        ;
assignment_target: expression
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
                {
                    // Create ST record
                    tabulate::id_symtrec id_rec;
                    id_rec.level = drv.level;
                    // Add to ST
                    drv.symtab_id.insert($1, id_rec, drv.active_func_stack);
                }
             | ID COMMA variable_list
             {
                // Create ST record for the first ID in the list
                tabulate::id_symtrec id_rec;
                id_rec.level = drv.level;
                // Add the first ID to ST
                drv.symtab_id.insert($1, id_rec, drv.active_func_stack);
                // No explicit action for variable_list as it's handled in the recursive call
             }
             ;
parameter_list: /* empty */ { }
              | variable_list {$$ = $1}
              ;
function_definition: FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS function_body ;
function_body: OPEN_CURLY statement_list return_stmt CLOSE_CURLY ;
%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}