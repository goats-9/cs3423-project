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
    struct param_symtrec;
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

/* Nonterminals */
%nterm <std::vector<std::string>> parameter_list ID_list
%nterm <bool> statement statement_list

%%
%start S;
// last rule to get reduced (for translation purpose)
S : program ;

// list of program elements
program: /* empty */
        | program program_element
        ; 

// program element
program_element: function_definition
              | struct_declaration
                {
                    drv.scope_level--;
                    tabulate::dtype_symtrec struc;
                    struc.level = drv.scope_level;
                    int res = drv.symtab_dtype.insert($1, struc, drv.symtab_func, drv.active_func_stack);
                    if (res == -1) {exit(res);}
                }
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
args : /* empty */
     | expression_list
     ;
function_call: instance OPEN_PARENTHESIS args CLOSE_PARENTHESIS  
             | ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS 
             ;
/* function call ends */

/* struct defination starts */
struct_declaration: STRUCT ID OPEN_CURLY struct_member_list CLOSE_CURLY SEMICOLON
                {
                    drv.scope_level--;
                    
                    tabulate::dtype_symtrec struc;
                    struc.level = drv.scope_level;

                    int res = drv.symtab_dtype.insert($2, struc, drv.symtab_func, drv.active_func_stack);
                    if (res == -1) {
                        std::cerr << "Error: Failed to insert struct into symbol table." << std::endl;
                        exit(res);
                    }
                    $$ = $2;
                }
                ;
struct_member_list: /* empty */
                  | struct_member_list declaration_stmt
                  | struct_member_list function_definition
                  ;
/* struct defination ends */

// expression
expression
    : constant
    | variable
        {
            tabulate::id_symtrec &var_record = drv.symtab_id.find($1, drv.scope_level);
            if (var_record == NULL) {
                std::cerr << "Error: Undefined variable '" << $1 << "'." << std::endl;
                exit(EXIT_FAILURE);
            }
            $$ = var_record;
        }
    | UNIOP expression
    | expression BIOP expression
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
    | function_call
        {
            tabulate::func_symtrec &func_record = drv.symtab_func.find($1.name, drv.scope_level);
            if (func_record == NULL) {
                std::cerr << "Error: Undefined function '" << $1.name << "'." << std::endl;
                exit(EXIT_FAILURE);
            }
            $$ = func_record;
        }
    ;


// expression list
expression_list: expression
               | expression COMMA expression_list
               ;

// statement
statement: declaration_stmt { $$ = false; }
         | assignment_stmt { $$ = false; }
         | compound_statement { $$ = false; }
         | return_stmt {$$ = true;}
         | conditional_stmt { $$ = false; }
         | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement { $$ = false; }
         | BREAK SEMICOLON { $$ = false; }
         | CONTINUE SEMICOLON { $$ = false; }
         ;

// return statement
return_stmt: RETURN expression SEMICOLON
           | RETURN SEMICOLON
           ;

// list of statement list
statement_list: /* empty */ { $$ = false; }
              | statement_list statement { $$ = $1 || $2; }
              ; 

// compound statement
compound_statement: OPEN_CURLY statement_list CLOSE_CURLY;

// variable list
ID_list: ID 
        {
            /* Accumulate IDs */
            $$.push_back($1);
        }
        | ID COMMA ID_list
        {
            /* Accumulate IDs */
            $$ = $3;
            $$.push_back($1);
        }
        ;
parameter_list: /* empty */ { drv.scope_level++; }
            | ID_list 
            { 
                drv.scope_level++;
                for (auto u : $1) {
                    // Create ID record to insert into ST
                    tabulate::id_symtrec rec;
                    rec.level = 
                    // Insert into ST
                    drv.symtab_id.insert
                } 
            }
            ;

/* function definition starts */
function_definition: function_head OPEN_CURLY statement_list CLOSE_CURLY
                     {
                        if (!$3){
                            std::cerr << "Error: No return statement '" << $3.name << "'." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        /* level reduced by 2, since it was increased for parameter_list and function body */
                        drv.scope_level -= 2;     
                        /* delete ST entries */
                        drv.symtab_id.delete_scope(drv.active_func_stack, drv.scope_level);
                     }
                   ;
function_head: FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS
               {
                    /* insert function into ST */
                    tabulate::func_symtrec frec;
                    /* scope_level was incremented in parameter_list */
                    frec.level = drv.scope_level - 1;
                    frec.paramlist = $4;
                    int res = drv.symtab_func.insert($2, frec, drv.symtab_func, drv.active_func_stack);
                    if (res == -1) {
                        error(yyloc, "Function '" + $2 + "' already exists in the symbol table");
                        exit(EXIT_FAILURE);
                    }
                    /* increment scope_level for function body */
                    drv.scope_level++;
               }
             ;
/* function definition ends */

%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}