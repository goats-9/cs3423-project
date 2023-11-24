#pragma once
#include <string>
#include <vector>
#include <stdexcept>

class any;
class pos;
// unary function error message
std::runtime_error uni_err(const std::string &func, const any &a);

// binary function error message
std::runtime_error bi_err(const std::string &func, const any &a, const any &b);

// splits string with deliminter del
std::vector<std::string> split(std::string s, std::string del);

// converts any to int if possible
int to_int(const any &a, const pos &p);

// converts any to bool if possible
int to_bool(const any &a, const pos &p);

// displays runtime error message
void disp_error(const std::runtime_error &e);

// finding whether the type is inbuilt
bool isInbuilt(const std::string &t);
bool isPrimitive(const std::string &t);