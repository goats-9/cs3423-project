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
    struct id_symtrec;
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
    NEW "new_token"

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
%nterm 
    <std::vector<std::string>> decl_list parameter_list ID_list
    <bool> statement statement_list compound_statement
    <int> declare expression_list args
    <std::string> decl_item variable
    <tabulate::id_symtrec> expression

%%
%start S;
// last rule to get reduced (for translation purpose)
S: program;

// list of program elements
program: 
    /* empty */
    | program program_element
    ; 

// program element
program_element: 
    function_definition
    | struct_declaration
    ;

// array initializer
array_initializer: 
    OPEN_SQUARE_BRAC expression_list CLOSE_SQUARE_BRAC
    ;

// all constants
constant: 
    INT
    | STRING
    | BOOL
    | DOUBLE
    | DATE
    | TIME
    | RANGE
    | array_initializer
    ;

// declaring tokens
declare: 
    LET {$$ = TABULATE_LET;}
    | CONST {$$ = TABULATE_CONST;}
    ;

/* declaration statement starts */
declaration_stmt: 
    declare decl_list SEMICOLON
    {
        /**
         * 1. (SATG) Collect info here.
         * 2. Add to symbol table. 
        */
        for (auto var : $2) {
            tabulate::id_symtrec idrec;
            idrec.level = drv.scope_level;
            idrec.modifier = $1;
            drv.symtab_id.insert(var, idrec, drv.active_func_ptr);
        }
    }
    ;

decl_list: 
    decl_item
    {
        $$.push_back($1);
    }
    | decl_list COMMA decl_item
    {
        $$ = $1;
        $$.push_back($3);
    }
    ;

decl_item: 
    ID { $$ = $1; }
    | ID EQUAL expression { $$ = $1; }
    ;

/* declaration statement ends */

// assignment statement
assignment_stmt: 
    variable EQUAL expression SEMICOLON
    {
        // Check modifier of variable
        auto idrec = drv.symtab_id.find($1, drv.scope_level);
        if (idrec.modifier == TABULATE_CONST) {
            throw yy::parser::syntax_error(loc, "Cannot assign variable " + $1 + " marked as constant.");
        }
    }
    | variable EQUAL expression COMMA assignment_stmt
    {
        // Check modifier of variable
        auto idrec = drv.symtab_id.find($1, drv.scope_level);
        if (idrec.modifier == TABULATE_CONST) {
            throw yy::parser::syntax_error(loc, "Cannot assign variable " + $1 + " marked as constant.");
        }
    }
    ;

/* conditional statement starts */
if_stmt: 
    IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
    ;
elif_stmt: 
    ELSE IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement
    ;
list_of_elif: /* empty */
    | list_of_elif elif_stmt
    ;
else_stmt: 
    ELSE compound_statement 
    ;
conditional_stmt: 
    if_stmt list_of_elif
    | if_stmt list_of_elif else_stmt
    ;
/* conditional statement ends */

// instances
instance: 
    expression DOT ID
    ;

/* accessing arrays and table expressions starts */
accessors: 
    accessor
    | accessors accessor
    ;
accessor: 
    OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC
    ;
/* accessing arrays and table expressions ends */

// variables
variable:
    ID 
    {
        // Check if the ID exists in the symbol table
        auto record = id_symtab.find($1, current_level);
        if (record == NULL) {
            throw yy::parser::syntax_error(loc, "error: identifier " + $1 + " not found."); 
        }
        $$ = $1;
    }
    | ID accessors 
    {
        // Check if the ID exists in the symbol table
        auto record = id_symtab.find($1, current_level);
        if (record == NULL) {
            throw yy::parser::syntax_error(loc, "error: identifier " + $1 + " not found."); 
        }
        $$ = $1;
    }
    | instance
    ;

/* function call starts */
args:
    /* empty */ {$$ = 0;}
    | expression_list {$$ = $1;}
    ;
function_call: 
    instance OPEN_PARENTHESIS args CLOSE_PARENTHESIS  
    | ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS
    {
        auto frec = drv.symtab_func.find($1, drv.scope_level);
        if (!frec) {
            throw yy::parser::syntax_error(loc, "error: couldn't find function " + $1);
        }
        if ((int)frec.paramlist.size() != $3) {
            throw yy::parser::syntax_error(loc, "error: incorrect number of arguments for function " + $1);
        }
    }
    ;
/* function call ends */

/* struct definition starts */
struct_declaration: 
    STRUCT ID OPEN_CURLY 
    {
        drv.scope_level++;
    }
    struct_member_list CLOSE_CURLY SEMICOLON
    {
        drv.scope_level--;
        
        tabulate::dtype_symtrec struc;
        struc.level = drv.scope_level;

        int res = drv.symtab_dtype.insert($2, struc, drv.symtab_func, drv.active_func_stack);
        if (res == -1) {
            std::cerr << "Error: Failed to insert struct into symbol table." << std::endl;
            exit(res);
        }
    }
    ;
struct_member_list:
    /* empty */
    | struct_member_list declaration_stmt
    | struct_member_list function_definition
    ;
/* struct definition ends */

// expression
expression: 
    constant
    | variable
    {
        tabulate::id_symtrec var_record = drv.symtab_id.find($1, drv.scope_level);
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
    | NEW ID OPEN_PARENTHESIS CLOSE_PARENTHESIS
    ;

// expression list
expression_list: 
    expression { $$ = 1; }
    | expression COMMA expression_list {$$ = 1 + $3; }
    ;

// statement
statement: 
    declaration_stmt { $$ = false; }
    | assignment_stmt { $$ = false; }
    | compound_statement { $$ = false; }
    | return_stmt {$$ = true;}
    | conditional_stmt { $$ = false; }
    | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS compound_statement { $$ = false; }
    | BREAK SEMICOLON { $$ = false; }
    | CONTINUE SEMICOLON { $$ = false; }
    ;

// return statement
return_stmt: 
    RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

// list of statement list
statement_list: 
    /* empty */ { $$ = false; }
    | statement_list statement { $$ = $1 || $2; }
    ; 

// compound statement
compound_statement: 
    OPEN_CURLY
    {
        drv.scope_level++;
    } 
    statement_list CLOSE_CURLY 
    {
        $$ = $3;
        drv.scope_level--;
    }
    ;

// variable list
ID_list: 
    ID 
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

parameter_list: 
    /* empty */
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
function_definition: 
    function_head compound_statement
    {
        if (!$2) {
            throw yy::parser::syntax_error(loc, "error: no return statement in function.");
            exit(EXIT_FAILURE);
        }
        /* level reduced by 2, since it was increased for parameter_list and function body */
        drv.scope_level -= 2;     
        /* delete ST entries */
        drv.symtab_id.delete_scope(drv.active_func_stack, drv.scope_level);
    }
    ;
function_head: 
    FUN ID OPEN_PARENTHESIS 
    {
        /* Mid-rule action */
        drv.scope_level++;
    } 
    parameter_list CLOSE_PARENTHESIS
    {
        /* insert function into ST */
        tabulate::func_symtrec frec;
        /* scope_level was incremented in parameter_list */
        frec.level = drv.scope_level - 1;
        frec.paramlist = $5;
        int res = drv.symtab_func.insert($2, frec, drv.symtab_func, drv.active_func_stack);
        if (res == -1) {
            throw yy::parser::syntax_error(loc, "Function '" + $2 + "' already exists in the symbol table");
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