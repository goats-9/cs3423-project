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
#include "types.hh"
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
    <tabulate::constant> INT "integer"
    <tabulate::constant> STRING "string"
    <tabulate::constant> BOOL "boolean"
    <tabulate::constant> DOUBLE "double"
    <tabulate::constant> DATE "date"
    <tabulate::constant> TIME "time"
    <tabulate::constant> RANGE "range"

%nterm 
    <tabulate::constant> 
    constant
    array_initializer
    <std::string> 
    expression
    accessor
    accessors
    variable
    instance
    declare
    decl_item
    decl_list
    declaration_stmt
    expression_list
    statement
    statement_list
    function_definition
    function_decl
    ID_list
    parameter_list
    program_element
    program
    function_call
    args
    return_stmt
    assignment_stmt
    compound_statement

%%
%start S;
// last rule to get reduced (for translation purpose)
S : program {std::cout << $1 << "\n";};

// list of program elements
program: /* empty */ 
        {
            // translation
            $$ << "";
        }
        | program program_element 
        {
            // translation
            $$ << $1 << "\n" << $2;
        }
        ; 

// program element
program_element: function_definition
              | struct_declaration
              ;

// array initializer
array_initializer: OPEN_SQUARE_BRAC expression_list CLOSE_SQUARE_BRAC 
                 { 
                    // translation
                    $$.value << "{" << $2 << "}"; 
                    $$.type = "array";
                 };

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
declare: LET {$$ << "any";}
       | CONST {$$ << "const any";}
       ;

/* declaration statement starts */
declaration_stmt: declare decl_list SEMICOLON 
                {
                    // translation
                    $$ << $1 << " " << $2 << ";";
                };
decl_item: ID
         | ID EQUAL expression 
         {
            // translation
            $$ << $1 << " = " << $3;
         }
         ;
decl_list: decl_item
         | decl_list COMMA decl_item 
         {
            // translation
            $$ << $1 << "," << $3;
         }
         ;
/* declaration statement ends */

// assignment statement
assignment_stmt: 
    variable EQUAL expression SEMICOLON
    {
        // translation
        $$ << $1 << " = " << $3 << ";";
    } ;

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
instance: expression DOT ID 
        {
            // translation
            $$ << $1 << "." << $3;
        };

/* accessing arrays and table expressions starts */
accessors : accessor
         | accessors accessor 
         {
            // translation
            $$ << $1 << $2;
         }
         ;
accessor: OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC { $$ << "[" << $2 << "]"; };
/* accessing arrays and table expressions ends */

// variables
variable: ID 
        | ID accessors {$$ << $1 << $2;}
        | instance
        ;

/* function call starts */
args : /* empty */ 
     {
        // translation
        $$ << "";
     }
     | expression_list
function_call: 
    instance OPEN_PARENTHESIS args CLOSE_PARENTHESIS 
    {
        // translation
        $$ << $1 << "(" << $3 << ")"; 
    } | 
    ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS 
    { 
        // translation
        $$ << $1 << "(" << $3 << ")"; 
    }
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
            {
                // translation
                $$ << "any(" << "new " << $1.type << "(" << $1.value << ")" << "," << "\"" << $1.type << "\"" << ")";
            }
          | variable 
          | UNIOP expression
          | expression BIOP expression
          | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
          {
                // translation
                $$ << "(" << $2 << ")";
          }
          | function_call
          ;

// expression list
expression_list: expression
               | expression COMMA expression_list 
               {
                    // translation
                    $$ << $1 << "," << $3;
               }
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
         | expression SEMICOLON
         ;

// return statement
return_stmt: 
    RETURN expression SEMICOLON 
    {
        // translation
        if (!drv.isMain)
        {
            $$ << "return " << $2 << ";"; 

        }
    } 
    | 
    RETURN SEMICOLON 
    {
        // translation
        if (!drv.isMain)
        {
            $$ << "return any();";
        }
    }
    ;

// list of statement list
statement_list: /* empty */ 
              {
                // translation
                $$ = "";
              }
              | statement_list statement 
              {
                // translation
                $$ << $1 << "\n" << $2 ;
              }
              ; 

// compound statement
compound_statement: 
    OPEN_CURLY statement_list CLOSE_CURLY 
    {
        // translation
        $$ << "{" << $2 << "}";
    };

// variable list
ID_list: ID 
        {
            Create ST record
            tabulate::id_symtrec id_rec;
            id_rec.level = drv.level;
            // Add to ST
            drv.symtab_id.insert($1, id_rec, drv.active_func_stack);
            // translation
            $$ << "any " <<  $1 ;
        }
        | ID COMMA ID_list
        {
            Create ST record for the first ID in the list
            tabulate::id_symtrec id_rec;
            id_rec.level = drv.level;
            // Add the first ID to ST
            drv.symtab_id.insert($1, id_rec, drv.active_func_stack);
            // No explicit action for variable_list as it's handled in the recursive call
            // translation
            $$ << "any " <<  $1 << "," << $3 ;
        }
        ;
parameter_list: /* empty */ 
              {
                // translation 
                $$ << ""; 
              }
              | ID_list 
              ;

/* function defination starts */
function_definition: 
    function_decl OPEN_CURLY statement_list CLOSE_CURLY 
    {
        // translation of function body
        if (drv.isMain)
        {
            $$ 
                << $1 << "{\n"
                << "try{"
                << $3
                << "}\n"
                << "catch{\n"
                << "cerr << e.what() << \"\\n\";\n"
                << "return 1;\n"
                << "}\n"
                << "return 0;\n" 
                << "}"
            ;
        }
        else 
        {
            $$ 
                << $1
                << "{" 
                << $3
                << "\n}"
            ;
        }
    };
function_decl:
    FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS
    {
        // translation of function declaration
        if($2 == "main") 
        {
            drv.isMain = true;
            $$ 
                << "int " << $2 << "(" << $4 << ")";
        }
        else
        {
            drv.isMain = false;
            $$ 
                << "any " << $2 << "(" << $4 << ")" ;
        }
    }
/* function defination ends */

%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}