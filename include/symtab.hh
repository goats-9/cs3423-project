#pragma once

/**
 * Symbol Table API for Tabulate
 *
 * Symbol Table Structure
 * ----------------------
 *
 * The symbol table is implemented as a Hash table (C++ unordered_map). Each 
 * key-value pair of the map represents a symbol table entry indexed by the 
 * name of the identifiers, functions, datatypes, etc. Each entry in the symbol
 * table, is implemented as a stack, to handle nested scopes.
 *
 * Symbol Table Record Structure
 * -----------------------------
 *
 * Depending on the data to be stored in the symbol table, the symbol table
 * records can be of various types, as shown below.
 *
 * ===================================================
 * TOKEN      : SYMBOL TABLE RECORD CONTENTS
 * ===================================================
 * IDENTIFIER : NAME || LEVEL   | MODIFIER   |
 * FUNCTION   : NAME || RETTYPE | PARAMETERS | LEVEL |
 * DATATYPE   : NAME || LEVEL   | 
 * ===================================================
 *
*/

/* Standard C++ includes */
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_map>

/* Header defines */
#define TABULATE_LET 0
#define TABULATE_CONST 1

/**
 * Define symbol table and record types in a namespace to prevent clashes with
 * other namespaces 
 */
namespace tabulate {
    
    /* Class definitions for tabulate symbol table records */

    struct id_symtrec {
        /// @brief Scope of declaration.
        int level;
        int modifier;
    };

    struct func_symtrec {
        int level;
        std::vector<std::string> paramlist;
    };

    struct dtype_symtrec {
        int level;
        std::vector<int> constr_args;
    };

    /**
     * Symbol Table class template
    */
    template<typename K, typename V>
    class symtab {
    public:
        std::unordered_map<K, std::stack<V>> tabulate_symtab;
        symtab() {}

        int insert(
            K &name, 
            V &rec,
            func_symtrec &active_func_rec
        ) {
            if (tabulate_symtab.find(name) == tabulate_symtab.end()) {
                if (!std::is_same<V, func_symtrec>::value) {
                    int func_level = active_func_rec.level;
                    std::vector<std::string> active_func_params = active_func_rec.paramlist;
                    for (auto param : active_func_params) {
                        if (func_level + 1 == rec.level && param == name) return -1;
                    }
                }
                tabulate_symtab[name].push(rec);
                return 0;
            } else return -1;
        }
        
        V find(K &name, int level) {
            V err;
            err.level = -1;
            if (tabulate_symtab.find(name) == tabulate_symtab.end()) return err;
            else {
                if (tabulate_symtab[name].top().level > level) return err;
                return tabulate_symtab[name].top();
            }
        }
        
        void delete_scope(int level) {
            for (auto &[name, stk] : tabulate_symtab) {
                while (!stk.empty() && stk.top().level > level) stk.pop();
            }
        }
    };
}
