#ifndef DRIVER_HH
#define DRIVER_HH
#include <fstream>
#include <map>
#include "parser.tab.hh"
using namespace std;

// Prototype of yylex given to Flex
#define YY_DECL \
    yy::parser::symbol_type yylex(driver &drv)
YY_DECL;

// Parser driver for Tabulate
class driver
{
public:
    driver();

    // The name of the file being parsed
    string file;

    // Run the parser on file F.  Return 0 on success.
    int parse(const string &f);

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
    inline void handleToken(yy::parser::symbol_type token)
    {
        if (isLexOut)
        {
            cout << token.name() << "\n";
        }
    }

    // The token's location used by the scanner.
    yy::location location;

    // symbol table

    // other variable
    int result;
};
#endif  // DRIVER_HH
