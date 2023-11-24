#pragma once

#include <string>
#include <fstream>
#include <map>
#include "parser.tab.hh"
#include "symtab.hh"
#include "translation.hh"

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

        // remove extension
        std::string remove_extension();

        // compiles the translated c++ code
        int compile();

        // Run the parser on file F.  Return 0 on success.
        int parse(const std::string &f);

        // Whether to generate scanner debug traces.
        bool trace_scanning;

        // Whether to generate parser debug traces.
        bool trace_parsing;

        // Whether to generate lexer output
        bool isLexOut;

        std::ofstream outFile;

        translation trans;

        // Handling the scanner.
        // Return 0 on success
        int scan_begin();
        void scan_end();

        // handles tokens here
        void handleToken(yy::parser::symbol_type token, const std::string & text);

        // The token's location used by the scanner.
        yy::location location;

        // symbol table
        func_symtrec active_func_ptr;
        int scope_level, while_level, num_main;
        std::string current_struct;
        bool in_struct;
        bool in_main;
        bool in_func;
        bool in_construct;
        symtab<std::string, id_symtrec> symtab_id;
        symtab<std::string, func_symtrec> symtab_func;
        symtab<std::string, dtype_symtrec> symtab_dtype;
        std::map<std::string,std::string> ID_mapping_struct_func;

        driver();
        void symtab_init();
        void delete_scope();

        template <typename K, typename V>
        V find(K &name) {
            V ret;
            ret = symtab_id.find(name);
            if (ret.level > -1) return ret;
            ret = symtab_dtype.find(name);
            if (ret.level > -1) return ret;
            ret = symtab_func.find(name);
            if (ret.level > -1) return ret;
            return ret;
        }
    };
}
