#include "symtab.hh"

template <typename K, typename V>
int tabulate::symtab<K, V>::insert(K &name, V &rec) {
    if (tabulate_symtab.find(name) == tabulate_symtab.end()) {
        if (std::is_same<V, func_symtrec>) {
            active_func_stack.push(name);
        } else {
            std::string active_func_name = active_func_stack.top();
            std::vector<param_symtrec> active_func_params = tabulate_symtab[name].top();
            for (param_symtrec param : active_func_params) {
                if (param.level + 1 == rec.level && param.name == name) return -1;
            }
        }
        tabulate_symtab[name].push(rec);
        return 0;
    } else return -1;
}

template <typename K, typename V>
V tabulate::symtab<K, V>::find(K &name) {
    if (tabulate_symtab.find(name) == tabulate_symtab.end()) return NULL;
    else {
        if (tabulate_symtab[name].top().level > _level) return NULL;
        return tabulate_symtab[name].top();
    }
}

template <typename K, typename V>
void tabulate::symtab<K, V>::delete_scope() {
    for (auto &[name, stk] : tabulate_symtab) {
        while (!stk.empty() && stk.top().level == _level) {
            if (active_func_stack.top() == name) active_func_stack.pop();
            stk.pop();
        }
    }
}