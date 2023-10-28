#ifndef SYMTAB_HH
#define SYMTAB_HH

#define N_SCOPES 127
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
 * ============================================================
 * TOKEN      : SYMBOL TABLE RECORD CONTENTS
 * ============================================================
 * IDENTIFIER : NAME | RECTYPE | TYPE    | ELETYPE    | LEVEL | 
 * FUNCTION   : NAME | RECTYPE | RETTYPE | PARAMETERS | LEVEL | 
 * DATATYPE   : NAME | RECTYPE | LEVEL   | 
 * ============================================================
 *
*/

/* Standard C++ includes */
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <variant>
#include <unordered_map>

/**
 * Define symbol table and record types in a namespace to prevent clashes with
 * other namespaces 
 */
namespace tabulate {
    
    /* Class definitions for tabulate symbol table records */

    struct id_symtrec {
        /// @brief Type of token.
        int rectype;
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
        int rectype, rettype, level;
        std::vector<id_symtrec> params;
    };

    struct dtype_symtrec {
        int rectype, level;
    };

    /**
     * Symbol Table clas template
    */
    template<typename T>
    class symtab {
    private:
        std::unordered_map<std::string, std::stack<T>> tabulate_symtab;
        int _level;
        void delete_scope();
    public:
        symtab() : _level(0) {}

        int insert(std::string &name, T &rec);
        T find(std::string &name);
        void begin_scope();
        void end_scope();
    };
}

#endif  // SYMTAB_HH
