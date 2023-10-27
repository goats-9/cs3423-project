#include "auxilary.hh"
using namespace std;

// write the function here
vector<string> helper::split(string s, string del)
{
    vector<string> res;
    int start, end = -1 * del.size();
    do
    {
        start = end + del.size();
        end = s.find(del, start);
        res.push_back(s.substr(start, end - start));
    } while (end != -1);

    return res;
}

helper::date::date() : year(0), month(0), day(0) 
{}
helper::date::date(string str)
{
    vector<string> temp = split(str, "-");
    year = stoi(temp[0]);
    month = stoi(temp[1]);
    day = stoi(temp[2]);
}

helper::time::time() : hour(0), min(0), sec(0)
{}
helper::time::time(string str)
{
    vector<string> temp = split(str, ":");
    hour = stoi(temp[0]);
    min = stoi(temp[1]);
    sec = stoi(temp[2]);
}