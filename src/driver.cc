#include "../include/driver.hh"
#include <iostream>
#include <cstring>

extern FILE *yyin;
extern int yy_flex_debug;

namespace tabulate
{
    driver::driver()
    {
        // Initialize variables
        trace_parsing = false;
        trace_scanning = false;
        isLexOut = false;
        scope_level = 0;
        while_level = 0;
        num_main = 0;
        driver::symtab_init();
    }

    int driver::parse(const std::string &f)
    {
        // setting file name
        file = f;
        // initialize the location
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
    
    int driver::scan_begin()
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

    void driver::scan_end()
    {
        fclose(yyin);
    }

    void driver::symtab_init() {
        // Initialize function symbol table with standard
        // functions offered by Tabulate
        std::vector<std::string> biop_func_names = 
        {
            "ADD",
            "SUB",
            "MUL",
            "DIV",
            "MOD",
            "POW",
            "BOR",
            "BAND",
            "BXOR",
            "BNOT",
            "BLS",
            "BRS",
            "AND",
            "OR",
            "XOR",
            "GET",
        };
        std::vector<std::string> uniop_func_names = 
        {
            "NOT",
            "TYPEOF",
            "LENGTH",
        };
        std::vector<std::string> biop_paramlist = {"p1","p2"};
        std::vector<std::string> uniop_paramlist = {"p1"};
        func_symtrec frec;
        frec.level = 0;
        frec.paramlist = biop_paramlist;
        for (auto u : biop_func_names) {
            symtab_func.tabulate_symtab[u].push(frec);
        }
        frec.paramlist = uniop_paramlist;
        for (auto u :uniop_func_names) {
            symtab_func.tabulate_symtab[u].push(frec);
        }
    }

    void driver::delete_scope() {
        driver::symtab_dtype.delete_scope(driver::scope_level);
        driver::symtab_func.delete_scope(driver::scope_level);
        driver::symtab_id.delete_scope(driver::scope_level);
    }
}
