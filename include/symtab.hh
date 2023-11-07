#pragma once

/**
 * Symbol Table API for Tabulate
 *
 * Symbol Table Structure
 * ----------------------
 *
 * The symbol table is implemented as a Red-Black tree (C++ map). Each key-value
 * pair of the map represents a symbol table entry indexed by the name of the
 * identifiers, functions, keywords, datatypes, etc. Each entry in the symbol
 * table, except the immutable pre-defined datatypes and operations, is
 * implemented as a stack, to handle nested scopes.
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

    struct param_symtrec {
        std::string name;
        /// @brief Scope of declaration.
        int level;
    };

    struct func_symtrec {
        int rettype, level;
        std::vector<param_symtrec> paramlist;
    };

    struct dtype_symtrec {
        int level;
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
            symtab<std::string, func_symtrec> &func_symtab,
            std::stack<std::string> &active_func_stack
        ) {
            if (tabulate_symtab.find(name) == tabulate_symtab.end()) {
                if (std::is_same<V, func_symtrec>::value) {
                    active_func_stack.push(name);
                } else {
                    std::string active_func_name = active_func_stack.top();
                    std::vector<param_symtrec> active_func_params = func_symtab.tabulate_symtab[active_func_name].top().paramlist;
                    for (param_symtrec param : active_func_params) {
                        if (param.level + 1 == rec.level && param.name == name) return -1;
                    }
                }
                tabulate_symtab[name].push(rec);
                return 0;
            } else return -1;
        }
        
        V find(K &name, int level) {
            if (tabulate_symtab.find(name) == tabulate_symtab.end()) return NULL;
            else {
                if (tabulate_symtab[name].top().level > level) return NULL;
                return tabulate_symtab[name].top();
            }
        }
        
        void delete_scope(std::stack<std::string> &active_func_stack, int level) {
            for (auto &[name, stk] : tabulate_symtab) {
                while (!stk.empty() && stk.top().level >= level) {
                    if (active_func_stack.top() == name) active_func_stack.pop();
                    stk.pop();
                }
            }
        }
    };
}
