#include "state.hh"

std::ostream& operator<<(std::ostream& o, state s)
{
    while (!s.loc.empty())
    {
        o << "From " << s.loc.top() << ": " << s.func_call.top() << "\n";
        s.loc.pop();
        s.func_call.pop();
    }
    return o;
}