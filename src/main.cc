#include <iostream>
#include "driver.hh"
using namespace std;

int main(int argc, char *argv[])
{
    int res = 0;
    driver drv;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == string("-p"))
            drv.trace_parsing = true;
        else if (argv[i] == string("-s"))
            drv.trace_scanning = true;
        else if (!drv.parse(argv[i]))
        {
            cout << drv.result << endl;
        }
        else
        {
            res = 1;
        }
    }

    return res;
}