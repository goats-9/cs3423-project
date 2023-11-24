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
    <std::string> ID "identifier_token"

// Operators
%left
    <std::string> UNIOP "unary operator"
%right
    <std::string> BIOP "binary operator"

// Constants
%token <std::string>
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
    <tabulate::function_info> function_definition function_head
    <tabulate::String> decl_item variable
    <tabulate::struct_member_list> struct_member_list
/* Nonterminals for translation */
%nterm
    <tabulate::instance> instance
    <tabulate::struct_info> struct_declaration
    <tabulate::program_element> program_element
    <tabulate::program> program
    <tabulate::constant> constant array_initializer constructor_call shape_initializer
    <tabulate::Default> expression function_call accessors assignment_stmt declaration_stmt return_stmt

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

        // declaring new datatypes in runtime function
        // Constructor
        drv.outFile
        << "void any::Constructor(const any &a)\n"
        << "{\n";
        for (auto i: $1.structList)
        {
            drv.outFile 
            << "if (a.type == \"" << i.name << "\"){\n"
            << "data = new " << i.name << "(*(" << i.name << " *)a.data);\n"
            << "type = \"" << i.name << "\";\n"
            << "}\n";
        }
        drv.outFile << "}\n"
        // Destructor()
        << "void any::Destructor(){\n";
        for (auto i: $1.structList)
        {
            drv.outFile 
            << "if (type == \"" << i.name << "\"){\n"
            << "delete (" << i.name << "*)data;\n"
            << "}\n";
        }
        drv.outFile << "}\n"
        // Accessor
        << "any &any::Accessor(const string &id){\n";
        for (auto i: $1.structList)
        {
            drv.outFile 
            << "if (type == \"" << i.name << "\"){\n"
            << i.name << " *ptr = (" << i.name << " *)data;\n"
            << "return ptr->mem[id];\n"
            << "}\n";
        }
        drv.outFile 
        << "throw uni_err(\"dot(.)\",*this);"
        << "}\n"
        // Runner()
        << "any any::Runner(const string &id,const vector<any> &params, const pos &p){\n";
        for (auto i: $1.structList)
        {
            drv.outFile 
            << "if (type == \"" << i.name << "\"){\n"
            << "any (" << i.name << "::*f)(vector<any>, const pos &);\n"
            << i.name << " *ptr = (" << i.name << " *)data;\n"
            << "__runnerCode\n"
            << "}\n";
        }
        drv.outFile 
        << "throw uni_err(\"dot(.)\",*this);\n"
        << "}\n";
    }
    ;

// list of program elements
program: 
    /* empty */
    {
        
    }
    | program program_element
    {
        $$ = $1;
        if ($2.type == TABULATE_STRUCT)
        {
            $$.structList.push_back($2.structInfo);
        }
    }
    ; 

// program element
program_element: 
    function_definition
    {
        $$.type = TABULATE_FUNC;
    }
    | struct_declaration
    {
        $$.type = TABULATE_STRUCT;
        $$.structInfo = $1;
    }
    ;

// array initializer
array_initializer: 
    OPEN_SQUARE_BRAC expression_list CLOSE_SQUARE_BRAC
    {
        $$.type = "array";
        $$.value << "vector<any>({" << $2.trans << "})";
    }
    ;

// shape intializer
shape_initializer:
    OPEN_PARENTHESIS expression COMMA expression CLOSE_PARENTHESIS
    {
        $$.type = "shape";
        $$.value << "shape(" << $2.trans << "," << $4.trans << ")";
    }

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
        $$ = $1;
    }
    |
    shape_initializer
    {
        $$ = $1;
    }
    | constructor_call 
    {
        $$ = $1;
    }
    ;

// declaring tokens
declare: 
    LET 
    {
        drv.assign_type = TABULATE_LET;
        $$.sem = TABULATE_LET;
        $$.trans = "any";
    }
    | CONST 
    {
        drv.assign_type = TABULATE_CONST;
        $$.sem = TABULATE_CONST;
        $$.trans = "const any";
    }
    ;

/* declaration statement starts */
declaration_stmt: 
    declare decl_list SEMICOLON
    {
        // translation
        $$.trans << $1.trans << " " << $2.trans << ";" ;
    }
    ;

decl_list: 
    decl_item
    {
        $$.trans = $1.trans;
    }
    | decl_list COMMA decl_item
    {
        // translation
        $$.trans << $1.trans << "," << $3.trans;
    }
    ;

decl_item: 
    ID 
    { 
        $$.trans = $1;
        tabulate::id_symtrec idrec;
        drv.find($1,idrec);
        if (idrec.level == -2)
        {
            throw yy::parser::syntax_error(@$, "error: Cannot assign variable " + $1 + " as it is defined already");
        }
        idrec.level = drv.scope_level;
        int res = drv.symtab_id.insert($1, idrec, drv.active_func_ptr);
        if (res == -1) {
            throw yy::parser::syntax_error(@$, "error: Cannot assign variable " + $1 + " as it is defined already");
        }
    }
    | ID EQUAL expression 
    { 
        $$.trans << $1 << " = " << $3.trans;
        tabulate::id_symtrec idrec;
        drv.find($1,idrec);
        if (idrec.level == -2)
        {
            throw yy::parser::syntax_error(@$, "error: Cannot assign variable " + $1 + " as it is defined already");
        }
        idrec.level = drv.scope_level;
        int res = drv.symtab_id.insert($1, idrec, drv.active_func_ptr);
        if (res == -1) {
            throw yy::parser::syntax_error(@$, "error: Cannot assign variable " + $1 + " as it is defined already");
        }
    }
    ;

/* declaration statement ends */

// assignment statement
assignment_stmt: 
    variable EQUAL expression SEMICOLON
    {
        // Check modifier of variable
        tabulate::id_symtrec idrec;
        drv.find($1.sem,idrec);
        if (idrec.modifier == TABULATE_CONST) {
            throw yy::parser::syntax_error(@$, "Cannot assign variable " + $1.sem + " marked as constant.");
        }

        // translation
        $$.trans << $1.trans << " = " << $3.trans << ";";
    }
    | variable EQUAL expression COMMA assignment_stmt
    {
        // Check modifier of variable
        tabulate::id_symtrec idrec;
        drv.find($1.sem,idrec);
        if (idrec.modifier == TABULATE_CONST) {
            throw yy::parser::syntax_error(@$, "Cannot assign variable " + $1.sem + " marked as constant.");
        }

        // translation
        $$.trans << $1.trans << " = " << $3.trans << "," << $5.trans;
    }
    ;

/* conditional statement starts */
if_stmt: 
    IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 
    {
        location loc(@1.begin,@4.end);
        drv.outFile 
        << "if (to_bool(" << $3.trans <<  "," << tabulate::translatePos(loc,"if predicate") << "))\n";
    }
    compound_statement
    ;
elif_stmt: 
    ELSE IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 
    {
        location loc(@1.begin,@5.end);
        drv.outFile 
        << "else if (to_bool(" << $4.trans <<  "," << tabulate::translatePos(loc,"else if predicate") <<"))\n";
    }
    compound_statement
    ;
list_of_elif: /* empty */
    | list_of_elif elif_stmt
    ;
else_stmt: 
    ELSE
    {
        drv.outFile 
        << "else\n";
    } 
    compound_statement 
    ;
conditional_stmt: 
    if_stmt list_of_elif
    | if_stmt list_of_elif else_stmt
    ;
/* conditional statement ends */

// instances
instance: 
    expression DOT ID
    {
        $$.exp = $1.trans;
        $$.attribute = $3;
    }
    | THIS DOT ID 
    {
        $$.exp = "this";
        $$.attribute = $3;
    }
    ;

/* accessing arrays and table expressions starts */
accessors: 
    OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC
    {
        $$.trans << ".at(" 
        << $2.trans << "," << tabulate::translatePos(@$,"[] operator")
        << ")";
    }
    | OPEN_SQUARE_BRAC expression CLOSE_SQUARE_BRAC accessors
    {
        $$.trans << ".at(" 
        << $2.trans << "," << tabulate::translatePos(@$,"[] operator")
        << ")" << $4.trans ;
    }
    ;
/* accessing arrays and table expressions ends */

// variables
variable:
    ID 
    {
        // Check if the ID exists in the symbol table
        tabulate::id_symtrec idrec;
        drv.find($1,idrec);
        if (idrec.level == -1) {
            throw yy::parser::syntax_error(@$, "error: couldn't find variable " + $1);
        }
        $$.sem = $1;

        // translation
        if (drv.ID_mapping_struct_func.find($1) != drv.ID_mapping_struct_func.end())
        {
            $$.trans << drv.ID_mapping_struct_func[$1];
        }
        else
        {
            $$.trans << $1;
        }
    }
    | expression accessors 
    {
        /* runtime semantic check */

        // translation
        $$.trans << $1.trans << $2.trans; 
    }
    | instance 
    { 
        /* runtime semantic check */ 

        // translation
        if ($1.exp == "this")
        {
            $$.trans << "mem[\"" << $1.attribute << "\"]";
        }
        else
        {
            $$.trans << $1.exp << ".access(\"" << $1.attribute << "\"," << tabulate::translatePos(@$,"dot(.)") << ")";
        }
    }
    ;

/* function call starts */
args:
    /* empty */ 
    {
        $$.sem = 0;
        $$.trans = "";
    }
    | expression_list 
    {
        $$.sem = $1.sem;
        $$.trans = $1.trans;
    }
    ;
function_call: 
    instance OPEN_PARENTHESIS args CLOSE_PARENTHESIS
    {
        // semantic check required for this.func

        // translation
        if ($1.exp == "this")
        {
            $$.trans
            << "this->" << $1.attribute << "(" << $3.trans << ")";
        }
        else
        {
            $$.trans 
            << $1.exp << ".run(\"" 
            << $1.attribute << "\"," << "{" << $3.trans << "}" << "," << tabulate::translatePos(@$,$1.attribute)
            << ")";
        }
    }  
    | ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS
    {
        tabulate::func_symtrec frec;
        drv.find($1,frec);
        if (frec.level == -1) {
            throw yy::parser::syntax_error(@$, "error: couldn't find function " + $1);
        }
        if ((int)frec.paramlist.size() != $3.sem) {
            throw yy::parser::syntax_error(@$, "error: incorrect number of arguments for function " + $1);
        }

        // translation
        if ($3.sem)
        {
            $$.trans
            << $1 << "(" << $3.trans << "," 
            <<  tabulate::translatePos(@$,$1) 
            << ")";
        }
        else
        {
            $$.trans
            << $1 << "(" << tabulate::translatePos(@$,$1) << ")" ;
        }
    }
    ;
/* function call ends */

/* Constructor call starts */
constructor_call:
    NEW ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS
    {
        tabulate::dtype_symtrec crec;
        drv.find($2,crec);
        if (crec.level == -1) {
            throw yy::parser::syntax_error(@$, "error: couldn't find struct " + $2);
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
        $$.type = $2;
        if ($4.sem == 0)
        {
            $$.value << $2 << "(" << tabulate::translatePos(@$,"new " + $2) << ")";
        }
        else
        {
            $$.value << $2 << "(" << $4.trans << "," << tabulate::translatePos(@$,"new " + $2) << ")";
        }
    };
/* Constructor call ends */

/* Constructor definition starts */
constructor_decl:
    CONSTRUCTOR OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS
    {
        // ++drv.scope_level;
        tabulate::id_symtrec idrec;
        idrec.level = drv.scope_level + 1;
        idrec.modifier = TABULATE_LET;
        for (auto name : $3.sem) {
            drv.symtab_id.insert(name, idrec, drv.active_func_ptr);
        }
        drv.active_func_ptr.level = drv.scope_level;
        drv.active_func_ptr.paramlist = $3.sem;
        $$.sem = $3.sem.size();
        drv.in_construct = true;

        // translation
        if ($3.sem.empty())
        {
            drv.outFile
            << drv.current_struct << "(const pos &p)\n";
        }
        else
        {
            drv.outFile
            << drv.current_struct << "(" << $3.trans << ",const pos &p)\n";
        }
    }
    ;
constructor_definition:
    constructor_decl compound_statement {
        // --drv.scope_level;
        drv.active_func_ptr.level = -1;
        drv.active_func_ptr.paramlist.clear();
        drv.delete_scope();
        $$.sem = $1.sem;
        drv.in_construct = false;
    }
    ;
/* Constructor definition ends */

/* struct definition starts */
struct_declaration: 
    STRUCT ID OPEN_CURLY 
    {
        drv.scope_level++;
        drv.in_struct = true;
        drv.current_struct = $2;

        // translation
        drv.outFile 
        << "class " << $2 << "{\n"
        << "public:\n"
        << "__funcMap<" << $2 << "> func;\n"
        << "__funcParams func_params;\n"
        << "__memMap mem;\n"; 
    }
    struct_member_list 
    {
        drv.scope_level--;
        drv.in_struct = false;

        location errLoc(@1.begin,@2.end);
        
        tabulate::dtype_symtrec struc;
        drv.find($2,struc);
        if (struc.level == -2) {
            throw yy::parser::syntax_error(errLoc, "Cannot create new struct " + $2 + " as it is defined already");
        }
        struc.level = drv.scope_level;
        struc.constr_args = $5.constr_args_list;
        int res = drv.symtab_dtype.insert($2, struc, drv.active_func_ptr);
        if (res == -1) {
            throw yy::parser::syntax_error(errLoc, "Cannot create new struct " + $2 + " as it is defined already");
        }
        // translation
        drv.outFile
        << "void func_decl(){\n";
        for (auto i: $5.func_in_struct)
        {
            drv.outFile
            << "func[\"" << i.name << "\"] = &" << $2 << "::" << i.name  << ";\n"
            << "func_params[\"" << i.name << "\"] = " << i.params << ";\n";
        }
        drv.outFile
        << "}\n";
        if ($5.constr_args_list.empty())
        {
            drv.outFile
            << $2 << "(const pos &p){\n"
            << "func_decl();\n"
            << "}\n";
        }
        drv.outFile
        << $2 << "(const " << $2 << " &a){\n"
        << "__copyConstruct\n"
        << "}\n";
    }
    CLOSE_CURLY SEMICOLON
    {
        $$.name = $2;
        // translation
        drv.outFile
        << "};\n";
    }
    ;
struct_member_list:
    /* empty */ { }
    | struct_member_list declaration_stmt { $$ = $1; }
    | struct_member_list function_definition { $$ = $1; $$.func_in_struct.push_back($2); }
    | struct_member_list constructor_definition { $$ = $1; $$.constr_args_list.push_back($2.sem); }
    ;
/* struct definition ends */

// expression
expression:
    constant
    {
        $$.trans 
        << "any(new " << $1.value << ",\"" << $1.type << "\")";
    }
    | variable
    {
        $$.trans = $1.trans;
    }
    | UNIOP expression
    {
        $$.trans 
        << $1 << "(" << $2.trans << "," 
        << tabulate::translatePos(@$,$1)
        << ")";
    }
    | expression BIOP expression
    {
        $$.trans 
        << $2 << "(" << $1.trans << "," << $3.trans  << "," 
        << tabulate::translatePos(@$,$2)
        << ")";
    }
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
    {
        $$.trans
        << "(" << $2.trans << ")";
    }
    | function_call
    {
        $$.trans << $1.trans;
    }
    ;

// expression list
expression_list: 
    expression 
    { 
        $$.sem = 1; 
        $$.trans = $1.trans;
    }
    | expression COMMA expression_list 
    {
        $$.sem = 1 + $3.sem; 
        $$.trans << $1.trans << "," << $3.trans ;
    }
    ;

// statement
statement: 
    declaration_stmt
    {
        drv.outFile << $1.trans << "\n";
    }
    | assignment_stmt
    {
        drv.outFile << $1.trans << "\n";
    }
    | compound_statement
    | return_stmt
    {
        drv.outFile << $1.trans << "\n";
    }
    | conditional_stmt
    | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 
    {
        /**
         * increase the while_level for checking
         * break and continue statements
        */ 
        ++drv.while_level;
        drv.outFile
        << "while (to_bool(" 
        << $3.trans << "," << tabulate::translatePos(@$,"while predicate")
        << "))";
    } 
    compound_statement 
    {
        --drv.while_level; 
    }
    | BREAK SEMICOLON 
    { 
        if (drv.while_level < 1) {
            throw yy::parser::syntax_error(@$, "error: break used outside a loop.");
        }

        drv.outFile << "break;\n";
    }
    | CONTINUE SEMICOLON
    { 
        if (drv.while_level < 1) {
            throw yy::parser::syntax_error(@$, "error: continue used outside a loop.");
        }

        drv.outFile << "continue;\n";
    }
    | function_call SEMICOLON
    {
        drv.outFile << $1.trans << ";\n";
    }
    ;

// return statement
return_stmt: 
    RETURN expression SEMICOLON
    {
        if (drv.in_main)
        {
            $$.trans << "return to_int(" 
            << $2.trans << "," <<  tabulate::translatePos(@$,"return in function main")
            << ");";
        }
        else
        {
            $$.trans 
            << "any ret_val = " << $2.trans << ";\n"
            << "st.outfunc();\n"
            << "return ret_val;";
        }
    }
    | RETURN SEMICOLON
    {
        if (drv.in_main)
        {
            throw yy::parser::syntax_error(@$, "main function cannot return none");
        }
        $$.trans << "return any();";
    }
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
        drv.outFile << "{\n"; 
        if (drv.in_func && !drv.in_main && drv.scope_level == 1)
        {
            drv.outFile << "st.infunc(p);\n";
        }
        if (drv.in_func && drv.in_struct && drv.scope_level == 2)
        {
            drv.outFile << "st.infunc(p);\n";
        }
        if (drv.in_construct && drv.scope_level == 2)
        {
            drv.outFile
            << "st.infunc(p);\n"
            << "func_decl();\n";
        }
    } 
    statement_list CLOSE_CURLY 
    {
        // translation of default return (except main)
        if (drv.in_func && !drv.in_main && drv.scope_level == 1)
        {
            drv.outFile 
            << "st.outfunc();\n"
            << "return any();\n";
        }
        if (drv.in_func && drv.in_struct && drv.scope_level == 2)
        {
            drv.outFile 
            << "st.outfunc();\n"
            << "return any();\n";
        }
        if (drv.in_construct && drv.scope_level == 2)
        {
            drv.outFile 
            << "st.outfunc();\n";
        }
        drv.scope_level--;
        drv.delete_scope();
        drv.outFile << "}\n"; 
    }
    ;

// variable list
ID_list: 
    ID 
    {
        /* Accumulate IDs */
        $$.sem.push_back($1);

        // translation
        $$.trans << "any " << $1;
    }
    | ID_list COMMA ID
    {
        /* Accumulate IDs */
        $$.sem = $1.sem;
        $$.sem.push_back($3);

        // translation
        $$.trans << $1.trans << "," << "any " << $3 ;
    }
    ;

parameter_list: 
    /* empty */ 
    { 
        $$.sem = {};
        $$.trans = ""; 
    }
    | ID_list 
    {
        $$.sem = $1.sem;
        $$.trans = $1.trans;
    }
    ;

/* function definition starts */
function_definition: 
    function_head compound_statement
    {
        /* level reduced by 1, since it was increased for parameter_list and function body */
        // drv.scope_level--;     
        /* delete ST entries */
        drv.delete_scope();
        /* cleanup */
        drv.active_func_ptr.level = -1;
        drv.active_func_ptr.paramlist.clear();

        if (drv.in_main)
        {
            // translation for return statement in main
            drv.outFile 
            << "catch(const runtime_error &e){\n"
            << "disp_error(e);\n"
            << "return 1;\n"
            << "}\nreturn 0;\n}";

            // updating drv.in_main
            drv.in_main = false;
        }

        // setting in_func false;
        drv.in_func = false;
        // clearing drv.ID_mapping_struct_func
        drv.ID_mapping_struct_func.clear();

        $$ = $1;
    }
    ;
function_head: 
    FUN ID OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS
    {
        /* insert function into ST */
        tabulate::func_symtrec frec;
        drv.find($2,frec);
        if (frec.level == -2) {
            throw yy::parser::syntax_error(@$, "error: Cannot create new function " + $2 + " as it is defined already");
        }
        /* scope_level was incremented in parameter_list */
        frec.level = drv.scope_level;
        frec.paramlist = $4.sem;
        int res = drv.symtab_func.insert($2, frec, drv.active_func_ptr);
        if (res == -1) {
            throw yy::parser::syntax_error(@$, "Function '" + $2 + "' already exists in the symbol table");
        }
        // ++drv.scope_level;
        // insert params into ST
        tabulate::id_symtrec idrec;
        idrec.level = drv.scope_level + 1;
        idrec.modifier = TABULATE_LET;
        for (auto u : $4.sem) {
            drv.symtab_id.insert(u, idrec, drv.active_func_ptr);
        }
        /* change active function pointer */
        drv.active_func_ptr = frec;
        $$.name = $2;
        $$.params = $4.sem.size();
        /* check for main function */
        if ($2 == "main") 
        {
            if (!$4.sem.empty())
            {
                throw yy::parser::syntax_error(@$, "Function '" + $2 + "' cannot have any parameter");
            }
            ++drv.num_main;
            drv.in_main = true;
        }
        drv.in_func = true;

        // translation
        if (drv.in_main)
        {
            drv.outFile 
            << "int main(int agrc, char* argv[])\n{\n"
            << "try\n";
        }
        else if (drv.in_struct)
        {
            drv.outFile << "any " << $2 << "(vector<any> params,const pos &p)\n";
            for(int i=0;i< (int)$4.sem.size();i++)
            {
                drv.ID_mapping_struct_func[$4.sem[i]] = "params[" + std::to_string(i) + "]";
            }
        }
        else
        {
            if ($4.sem.empty())
            {
                drv.outFile << "any " << $2 << "(const pos &p)\n";
            }
            else
            {
                drv.outFile << "any " << $2 << "(" << $4.trans << ",const pos &p)\n" ;
            }
        }
    }
    ;
/* function definition ends */

%%
void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
