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
 * IDENTIFIER : NAME | TOKTYPE | TYPE    | ELETYPE    | LEVEL | 
 * FUNCTION   : NAME | TOKTYPE | RETTYPE | PARAMETERS | LEVEL | 
 * DATATYPE   : NAME | TOKTYPE | LEVEL   | 
 * KEYWORD    : NAME | TOKTYPE | TYPE    | ELETYPE    |
 * ============================================================
 *
*/

/* Standard C++ includes */
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <string>

/**
 * Define symbol table and record types in a namespace to prevent clashes with
 * other namespaces.
 */
namespace tabulate_ST {
    
    /* Class definitions for tabulate symbol table records.*/

    struct tabulate_id_symtab {
        int toktype, type, eletype, level;
    };

    struct tabulate_func_symtab {
        int toktype, rettype, level;
        std::vector<tabulate_id_symtab> params;
    };

    struct tabulate_dtype_symtab {
        int toktype, level;
    };

    struct tabulate_keyword_symtab {
        int toktype, level;
    };

    std::map<std::string, int> tabulate_symtab;
}