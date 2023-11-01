#pragma once

#include <fstream>
#include <map>
#include <stack>
#include <string>
#include "../parser.tab.hh"

// Prototype of yylex given to Flex
#define YY_DECL \
    yy::parser::symbol_type yylex(tabulate::driver &drv)
// Declaring yylex for parser
YY_DECL;

namespace tabulate {
    // Parser driver for Tabulate
    class driver
    {
    public:
        driver();

        // The name of the file being parsed
        std::string file;

        // Run the parser on file F.  Return 0 on success.
        int parse(const std::string &f);

        // Whether to generate scanner debug traces.
        bool trace_scanning;

        // Whether to generate parser debug traces.
        bool trace_parsing;

        // Whether to generate lexer output
        bool isLexOut;

        // Handling the scanner.
        void scan_begin();
        void scan_end();

        // handles tokens here
        inline void handleToken(yy::parser::symbol_type token, const std::string & text)
        {
            if (isLexOut)
            {
                std::cout << token.name() << ": " << text << "\n";
            }
        }

        // The token's location used by the scanner.
        yy::location location;

        // symbol table
        std::stack<std::string> active_func_stack;
        int level;

        // other variable
        int result;
    };
}
