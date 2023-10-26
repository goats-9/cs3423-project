#ifndef AUXILARY_HH
#define AUXILARY_HH
#include "driver.hh"
#include "parser.tab.hh"
using namespace std;

namespace helper {
    // define helping functions here
    inline void handleToken(yy::parser::symbol_type token)
    {
        cout << token.name() << "\n";
    }
}

#endif