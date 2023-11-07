#pragma once

#include <string>
#include "parser.tab.hh"
#include "symtab.hh"

// Prototype of yylex given to Flex
#define YY_DECL yy::parser::symbol_type yylex(tabulate::driver &drv)
// Declaring yylex for parser
YY_DECL;

namespace tabulate {
    // Parser driver for Tabulate
    class driver
    {
    public:
        // The name of the file being parsed
        std::string file;

        // check file name
        bool check_extension();

        // Run the parser on file F.  Return 0 on success.
        int parse(const std::string &f);

        // Whether to generate scanner debug traces.
        bool trace_scanning;

        // Whether to generate parser debug traces.
        bool trace_parsing;

        // Whether to generate lexer output
        bool isLexOut;

        // Handling the scanner.
        // Return 0 on success
        int scan_begin();
        void scan_end();

        // handles tokens here
        void handleToken(yy::parser::symbol_type token, const std::string & text);

        // The token's location used by the scanner.
        yy::location location;

        // symbol table
        std::stack<std::string> active_func_stack;
        int level;
        tabulate::symtab<std::string, tabulate::id_symtrec> symtab_id;
        tabulate::symtab<std::string, tabulate::func_symtrec> symtab_func;
        tabulate::symtab<std::string, tabulate::dtype_symtrec> symtab_dtype;

        // other variable
        int result;

        driver();
    };
}
