#ifndef TABULATE_HH
#define TABULATE_HH

#include "tabulate/driver.hh"
#include "tabulate/symtab.hh"
#include "tabulate/types.hh"

tabulate::symtab<std::string, tabulate::id_symtrec> symtab_id;
tabulate::symtab<std::string, tabulate::func_symtrec> symtab_func;
tabulate::symtab<std::string, tabulate::dtype_symtrec> symtab_dtype;

void begin_scope();
void end_scope();

#endif // TABULATE_HH