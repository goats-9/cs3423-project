#include "../include/driver.hh"
#include <iostream>
#include <cstring>

extern FILE *yyin;
extern int yy_flex_debug;

namespace tabulate
{
    driver::driver()
    {
        // initilize variables
        trace_parsing = false;
        trace_scanning = false;
        isLexOut = false;
        scope_level = 0;
        while_level = 0;
    }
    int driver::parse(const std::string &f)
    {
        // setting file name
        file = f;
        // initilize the location
        location.initialize(&file);
        if (scan_begin())
            return 1;
        yy::parser parse(*this);
        parse.set_debug_level(trace_parsing);
        int res = parse();
        scan_end();
        return res;
    }
    bool driver::check_extension()
    {
        if (file.substr(file.find_last_of(".") + 1) == "tblt")
            return true;
        return false;
    }
    void driver::handleToken(yy::parser::symbol_type token, const std::string &text)
    {
        if (isLexOut)
        {
            std::cout << token.name() << ": " << text << "\n";
        }
    }
    int tabulate::driver::scan_begin()
    {
        yy_flex_debug = trace_scanning;
        // checking extension
        if (!check_extension())
        {
            std::cerr << "Enter a .tblt file\n";
            return 1;
        }
        // opening file
        if (!(yyin = fopen(file.c_str(), "r")))
        {
            std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
            return 1;
        }
        return 0;
    }
    void tabulate::driver::scan_end()
    {
        fclose(yyin);
    }
}
