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
    COMMA "comma_token"
    OPEN_SQUARE_BRAC "open_square_bracket_token" 
    CLOSE_SQUARE_BRAC "close_square_bracket_token"
    OPEN_CURLY "open_curly_bracket_token"
    CLOSE_CURLY "close_curly_bracket_token"
    OPEN_PARENTHESIS "open_parenthesis_token"
    CLOSE_PARENTHESIS "close_parenthesis_token"

%left 
    DOT "dot_token" 

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
%start S;
// last rule to get reduced
S : program ;

// list of program elements
program: /* empty */
        | program program_element
        ; 

// program element
program_element: function_definition
              | struct_declaration
              ;

// array initializer
array_initializer: OPEN_SQUARE_BRAC expression_list CLOSE_SQUARE_BRAC ;

// all constants
constant: INT
        | STRING
        | BOOL
        | DOUBLE
        | DATE
        | TIME
        | RANGE
        | array_initializer
        ;

// declaring tokens
declare: LET
       | CONST
       ;

/* declaration statement starts */
declaration_stmt: declare decl_list SEMICOLON ;
decl_item: ID
         | ID EQUAL expression
decl_list: decl_item
         | decl_list COMMA decl_item
/* declaration statement ends */

// assignment statement
assignment_stmt: variable EQUAL expression SEMICOLON
               | variable EQUAL expression COMMA assignment_stmt
               ;

/* conditional statement starts */
if_stmt: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement ;
elif_stmt: ELSE IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
list_of_elif: 
            | list_of_elif elif_stmt
else_stmt: ELSE compound_statement ;
conditional_stmt: if_stmt list_of_elif
                | if_stmt list_of_elif else_stmt
                ;
/* conditional statement ends */

// instances
instance: expression DOT ID ;

/* accessing arrays and table expressions starts */
accessors : accessor
         | accessors accessor
         ;
accessor: OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC ;
/* accessing arrays and table expressions ends */

// variables
variable: ID
        | ID accessors
        | instance
        ;

/* function call starts */
args : 
     | expression_list
function_call: instance OPEN_PARENTHESIS args CLOSE_PARENTHESIS  
             | ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS 
             ;
/* function call ends */

/* struct defination starts */
struct_declaration: STRUCT ID OPEN_CURLY struct_member_list CLOSE_CURLY SEMICOLON ;
struct_member_list: /* empty */
                  | struct_member_list declaration_stmt
                  | struct_member_list function_definition
                  ;
/* struct defination ends */

// expression
expression: constant
          | variable
          | UNIOP expression
          | expression BIOP expression
          | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
          | function_call
          ;

// expression list
expression_list: expression
               | expression COMMA expression_list
               ;

// statement
statement: declaration_stmt
         | assignment_stmt
         | compound_statement
         | return_stmt
         | conditional_stmt
         | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
         | function_definition
         | BREAK SEMICOLON
         | CONTINUE SEMICOLON
         ;

// return statement
return_stmt: RETURN expression SEMICOLON
           | RETURN SEMICOLON
           ;

// list of statement list
statement_list: /* empty */
              | statement_list statement
              ; 

// compound statement
compound_statement: OPEN_CURLY statement_list CLOSE_CURLY ;

// variable list
ID_list: ID 
                {
                    // Create ST record
                    tabulate::id_symtrec id_rec;
                    id_rec.level = drv.level;
                    // Add to ST
                    drv.symtab_id.insert($1, id_rec, drv.active_func_stack);
                }
             | ID COMMA ID_list
             ;

// parameter list
parameter_list: /* empty */
              | ID_list
              ;

/* function defination starts */
function_definition: FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS function_body ;
function_body: OPEN_CURLY statement_list CLOSE_CURLY ;
/* function defination ends */

%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}