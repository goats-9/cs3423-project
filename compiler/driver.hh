#ifndef DRIVER_HH
#define DRIVER_HH
#include <fstream>
#include <map>
#include "parser.tab.hh"
using namespace std;
#define YY_DECL \
    yy::parser::symbol_type yylex(driver &drv)
// Conducting the whole scanning and parsing of Calc++.
YY_DECL;
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

    // Handling the scanner.
    void scan_begin();
    void scan_end();

    // The token's location used by the scanner.
    yy::location location;

    // symbol table

    // other variable
    int res;
};
#endif
