class any;
#include <string>
#include <stdexcept>

// unary function error message
std::runtime_error uni_err(const std::string &func, const any &a);

// binary function error message
std::runtime_error bi_err(const std::string &func, const any &a, const any &b);