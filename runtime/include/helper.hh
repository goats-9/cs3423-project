#pragma once
#include <string>
#include <vector>
#include <stdexcept>

class any;
// unary function error message
std::runtime_error uni_err(const std::string &func, const any &a);

// binary function error message
std::runtime_error bi_err(const std::string &func, const any &a, const any &b);

// splits string with deliminter del
std::vector<std::string> split(std::string s, std::string del);