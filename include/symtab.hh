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
 * IDENTIFIER : NAME || TYPE    | ELETYPE    | LEVEL |
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

/**
 * Define symbol table and record types in a namespace to prevent clashes with
 * other namespaces 
 */
namespace tabulate {
    
    /* Class definitions for tabulate symbol table records */

    struct id_symtrec {
        /// @brief Determines whether the identifier is simple or is an array,
        /// taking values 0 or 1 respectively.
        int type;
        /// @brief Determine type of element of the identifier, meaningful for
        /// array identifiers.
        int eletype;
        /// @brief Scope of declaration.
        int level;
    };

    struct param_symtrec {
        std::string name;
        /// @brief Determines whether the identifier is simple or is an array,
        /// taking values 0 or 1 respectively.
        int type;
        /// @brief Determine type of element of the identifier, meaningful for
        /// array identifiers.
        int eletype;
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
     * Symbol Table clas template
    */
    template<typename K, typename V>
    class symtab {
    private:
        std::unordered_map<K, std::stack<V>> tabulate_symtab;
    public:
        symtab() {}

        int insert(K &name, V &rec, std::stack<std::string> &active_func_stack);
        V find(K &name, int level);
        void delete_scope(std::stack<std::string> &active_func_stack, int level);
    };
}
