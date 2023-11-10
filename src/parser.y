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
#include <vector>
namespace tabulate
{
    class driver;
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
    THIS "this_token"
    CONSTRUCTOR "constructor_token"
    LET "let_token"
    CONST "const_token"
    STRUCT "struct_token"
    IF "if_token"
    ELSE "else_token"
    VOID "void_token"
    WHILE "while_token"
    FUN "function_token"
    RETURN "return_token"
    BREAK "break_token"
    CONTINUE "continue_token"
    NEW "new_token"

// Punctuators
%token  
    EQUAL "equal_token" 
    COLON "colon_token" 
    SEMICOLON "semcolon_token"
    COMMA "comma_token"
    OPEN_CURLY "open_curly_bracket_token"
    CLOSE_CURLY "close_curly_bracket_token"
    OPEN_PARENTHESIS "open_parenthesis_token"
    CLOSE_PARENTHESIS "close_parenthesis_token"

%left 
    OPEN_SQUARE_BRAC "open_square_bracket_token" 
    CLOSE_SQUARE_BRAC "close_square_bracket_token"
    DOT "dot_token" 

// Identifiers
%token 
    <tabulate::Default> ID "identifier_token"

// Operators
%left
    <tabulate::Default> UNIOP "unary operator"
%right
    <tabulate::Default> BIOP "binary operator"

// Constants
%token <tabulate::Default>
    INT "integer"
    STRING "string"
    BOOL "boolean"
    DOUBLE "double"
    DATE "date"
    TIME "time"
    RANGE "range"

/* Nonterminals for semantic and translation*/
%nterm 
    <tabulate::vector_of_string> decl_list parameter_list ID_list
    <tabulate::Int> declare expression_list args constructor_decl constructor_definition
    <tabulate::String> decl_item variable
    <tabulate::vector_of_int> struct_member_list
/* Nonterminals for translation */
%nterm
    <tabulate::constant> constant array_initializer constructor_call

%%
%start S;
// last rule to get reduced (for translation purpose)
S: 
    program
    {
        // checking if main exist
        if (drv.num_main != 1) {
            yy::parser::syntax_error(@$, "error: there should be exactly one main function.");
        }

    }
    ;

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
    {
        // $$ << "{" << $2 << "}" ;
    }
    ;

// all constants
constant: 
    INT 
    {
        $$.type = "int";
        $$.value << "int(" << $1 << ")";
    }
    | STRING
    {
        $$.type = "string";
        $$.value << "string(" << $1 << ")";
    }
    | BOOL
    {
        $$.type = "bool";
        $$.value << "bool(" << $1 << ")";
    }
    | DOUBLE
    {
        $$.type = "double";
        $$.value << "double(" << $1 << ")";
    }
    | DATE
    {
        $$.type = "date";
        $$.value << "date(" << $1 << ")";
    }
    | TIME
    {
        $$.type = "time";
        $$.value << "time(" << $1 << ")";
    }
    | RANGE
    {
        $$.type = "range";
        $$.value << "range(" << $1 << ")";
    }
    | array_initializer 
    {
        // $$.type = "array";
        // $$.value << "vector<any>(" << $1 << ")";
    }
    | constructor_call 
    {
        // $$ = $1;
    }
    ;

// declaring tokens
declare: 
    LET {$$.sem = TABULATE_LET;}
    | CONST {$$.sem = TABULATE_CONST;}
    ;

/* declaration statement starts */
declaration_stmt: 
    declare decl_list SEMICOLON
    {
        /**
         * 1. (SATG) Collect info here.
         * 2. Add to symbol table. 
        */
        for (auto var : $2.sem) {
            tabulate::id_symtrec idrec;
            idrec.level = drv.scope_level;
            idrec.modifier = $1.sem;
            int res = drv.symtab_id.insert(var, idrec, drv.active_func_ptr);
            if (res == -1) {
                throw yy::parser::syntax_error(@$, "error: '" + var + "' previously declared.");
            }
        }
    }
    ;

decl_list: 
    decl_item
    {
        $$.sem.push_back($1.sem);
    }
    | decl_list COMMA decl_item
    {
        $$.sem = $1.sem;
        $$.sem.push_back($3.sem);
    }
    ;

decl_item: 
    ID { $$.sem = $1; }
    | ID EQUAL expression { $$.sem = $1; }
    ;

/* declaration statement ends */

// assignment statement
assignment_stmt: 
    variable EQUAL expression SEMICOLON
    {
        // Check modifier of variable
        auto idrec = drv.symtab_id.find($1.sem, drv.scope_level);
        if (idrec.modifier == TABULATE_CONST) {
            throw yy::parser::syntax_error(@$, "Cannot assign variable " + $1.sem + " marked as constant.");
        }
    }
    | variable EQUAL expression COMMA assignment_stmt
    {
        // Check modifier of variable
        auto idrec = drv.symtab_id.find($1.sem, drv.scope_level);
        if (idrec.modifier == TABULATE_CONST) {
            throw yy::parser::syntax_error(@$, "Cannot assign variable " + $1.sem + " marked as constant.");
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
    | THIS DOT ID 
    ;

/* accessing arrays and table expressions starts */
accessors: 
    OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC
    | OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC accessors
    ;
/* accessing arrays and table expressions ends */

// variables
variable:
    ID 
    {
        // Check if the ID exists in the symbol table
        auto record = drv.symtab_id.find($1, drv.scope_level);
        if (record.level == -1) {
            throw yy::parser::syntax_error(@$, "error: identifier " + $1 + " not found."); 
        }
        $$.sem = $1;
    }
    | expression accessors {/* runtime semantic check */}
    | instance { /* runtime semantic check */ }
    ;

/* function call starts */
args:
    /* empty */ {$$.sem = 0;}
    | expression_list {$$.sem = $1.sem;}
    ;
function_call: 
    instance OPEN_PARENTHESIS args CLOSE_PARENTHESIS  
    | ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS
    {
        auto frec = drv.symtab_func.find($1, drv.scope_level);
        if (frec.level == -1) {
            throw yy::parser::syntax_error(@$, "error: couldn't find function " + $1);
        }
        if ((int)frec.paramlist.size() != $3.sem) {
            throw yy::parser::syntax_error(@$, "error: incorrect number of arguments for function " + $1);
        }
    }
    ;
/* function call ends */

/* Constructor call starts */
constructor_call:
    NEW ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS
    {
        auto crec = drv.symtab_dtype.find($2, drv.scope_level);
        if (crec.level == -1) {
            throw yy::parser::syntax_error(@$, "error: couldn't find constructor " + $2);
        }
        bool errfl = true;
        for (auto u : crec.constr_args) {
            if (u == $4.sem) {
                errfl = false;
                break;
            }
        }
        /* default constructor has 0 args */
        if (errfl && !$4.sem) errfl = false;
        if (errfl) throw yy::parser::syntax_error(@$, "error: incorrect number of arguments for constructor " + $2);
        // translation
        // $$.type = $2;
        // $$.value << $2 << "(" << $4.trans << ")";
    };
/* Constructor call ends */

/* Constructor definition starts */
constructor_decl:
    CONSTRUCTOR OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS
    {
        ++drv.scope_level;
        tabulate::id_symtrec idrec;
        idrec.level = drv.scope_level;
        idrec.modifier = TABULATE_LET;
        for (auto name : $3.sem) {
            drv.symtab_id.insert(name, idrec, drv.active_func_ptr);
        }
        drv.active_func_ptr.level = drv.scope_level - 1;
        drv.active_func_ptr.paramlist = $3.sem;
        $$.sem = $3.sem.size();
    }
    ;
constructor_definition:
    constructor_decl compound_statement {
        --drv.scope_level;
        drv.active_func_ptr.level = -1;
        drv.active_func_ptr.paramlist.clear();
        drv.delete_scope();
        $$.sem = $1.sem;
    }
    ;
/* Constructor definition ends */

/* struct definition starts */
struct_declaration: 
    STRUCT ID OPEN_CURLY 
    {
        drv.scope_level++;
        drv.in_struct = true;
    }
    struct_member_list CLOSE_CURLY SEMICOLON
    {
        drv.scope_level--;
        drv.in_struct = false;
        
        tabulate::dtype_symtrec struc;
        struc.level = drv.scope_level;
        struc.constr_args = $5.sem;
        int res = drv.symtab_dtype.insert($2, struc, drv.active_func_ptr);
        if (res == -1) {
            throw yy::parser::syntax_error(@$, "error: failed to insert struct into symbol table.");
        }
    }
    ;
struct_member_list:
    /* empty */ { }
    | struct_member_list declaration_stmt { $$.sem = $1.sem; }
    | struct_member_list function_definition { $$.sem = $1.sem; }
    | struct_member_list constructor_definition { $$.sem = $1.sem; $$.sem.push_back($2.sem); }
    ;
/* struct definition ends */

// expression
expression:
    constant
    | variable
    | UNIOP expression
    | expression BIOP expression
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
    | function_call
    ;

// expression list
expression_list: 
    expression { $$.sem = 1; }
    | expression COMMA expression_list {$$.sem = 1 + $3.sem; }
    ;

// statement
statement: 
    declaration_stmt
    | assignment_stmt
    | compound_statement
    | return_stmt
    | conditional_stmt
    | WHILE OPEN_PARENTHESIS
    {
        /**
         * increase the while_level for checking
         * break and continue statements
        */ 
        ++drv.while_level;
    } 
    expression CLOSE_PARENTHESIS compound_statement 
    {
        --drv.while_level; 
    }
    | BREAK SEMICOLON 
    { 
        if (drv.while_level < 1) {
            throw yy::parser::syntax_error(@$, "error: break used outside a loop.");
        }
    }
    | CONTINUE SEMICOLON
    { 
        if (drv.while_level < 1) {
            throw yy::parser::syntax_error(@$, "error: continue used outside a loop.");
        }
    }
    | function_call SEMICOLON
    ;

// return statement
return_stmt: 
    RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

// list of statement list
statement_list: 
    /* empty */
    | statement_list statement
    ; 

// compound statement
compound_statement: 
    OPEN_CURLY
    {
        drv.scope_level++;
    } 
    statement_list CLOSE_CURLY 
    {
        drv.scope_level--;
        drv.delete_scope();
    }
    ;

// variable list
ID_list: 
    ID 
    {
        /* Accumulate IDs */
        $$.sem.push_back($1);
    }
    | ID COMMA ID_list
    {
        /* Accumulate IDs */
        $$.sem = $3.sem;
        $$.sem.push_back($1);
    }
    ;

parameter_list: 
    /* empty */ { $$.sem = {}; }
    | ID_list 
    {
        $$.sem = $1.sem;
    }
    ;

/* function definition starts */
function_definition: 
    function_head compound_statement
    {
        /* level reduced by 1, since it was increased for parameter_list and function body */
        drv.scope_level--;     
        /* delete ST entries */
        drv.delete_scope();
        /* cleanup */
        drv.active_func_ptr.level = -1;
        drv.active_func_ptr.paramlist.clear();
    }
    ;
function_head: 
    FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS
    {
        /* insert function into ST */
        tabulate::func_symtrec frec;
        /* scope_level was incremented in parameter_list */
        frec.level = drv.scope_level;
        frec.paramlist = $4.sem;
        int res = drv.symtab_func.insert($2, frec, drv.active_func_ptr);
        if (res == -1) {
            throw yy::parser::syntax_error(@$, "Function '" + $2 + "' already exists in the symbol table");
        }
        ++drv.scope_level;
        // insert params into ST
        tabulate::id_symtrec idrec;
        idrec.level = drv.scope_level;
        idrec.modifier = TABULATE_LET;
        for (auto u : $4.sem) {
            drv.symtab_id.insert(u, idrec, drv.active_func_ptr);
        }
        /* change active function pointer */
        drv.active_func_ptr = frec;
        /* check for main function */
        if ($2 == "main") ++drv.num_main;
    }
    ;
/* function definition ends */

%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
