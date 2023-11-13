#include "../include/types.hh"

std::string &yy::operator<<(std::string &s, std::string a)
{
    s = s + a;
    return s;
}
std::string &yy::operator<<(std::string &s, const char *a)
{
    s = s + a;
    return s;
}
