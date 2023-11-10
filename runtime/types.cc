#include "types.hh"
#include "any.hh"
using namespace std;

void cell::destroy()
{
    if (is_first)
    {
        delete val;
    }
}
void cell::construct(const cell &a)
{
    val = a.val;
    is_first = false;
}
void cell::construct(const any &a)
{
    val = new any(a);
}