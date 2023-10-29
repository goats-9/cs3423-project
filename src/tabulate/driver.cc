#include "driver.hh"

tabulate::driver::driver() : trace_parsing (false), trace_scanning (false), isLexOut(false)
{
    // initilize variables
}

int tabulate::driver::parse(const std::string &f)
{
    // setting file name
    file = f;
    // initilize the location
    location.initialize(&file);
    scan_begin();
    yy::parser parse(*this);
    parse.set_debug_level (trace_parsing);
    int res = parse();
    scan_end();
    return res;
}
