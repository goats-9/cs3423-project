#include "symtab.hh"

template <typename T>
int tabulate::symtab<T>::insert(std::string &name, T &rec) {
    if (tabulate_symtab.find(name) == tabulate_symtab.end()) {
        tabulate_symtab[name].push(rec);
        return 0;
    } else return -1;
}

template <typename T>
T tabulate::symtab<T>::find(std::string &name) {
    if (tabulate_symtab.find(name) == tabulate_symtab.end()) return NULL;
    else {
        if (tabulate_symtab[name].top().level > _level) return NULL;
        return tabulate_symtab[name].top();
    }
}

template <typename T>
void tabulate::symtab<T>::delete_scope() {
    for (auto &[_, stk] : tabulate_symtab) {
        while (!stk.empty() && stk.top().level == _level) stk.pop();
    }
}

template <typename T> 
void tabulate::symtab<T>::begin_scope() { ++_level; }

template <typename T>
void tabulate::symtab<T>::end_scope() { delete_scope(); --_level; }