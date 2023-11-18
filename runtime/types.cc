#include "types.hh"
#include "any.hh"
#include "helper.hh"
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
int table::read(std::string &path, char delim = ',') {
    fstream fin(path, std::ios_base::in);
    if (!fin) return -1; 
    // CSV handler: Read file line by line
    std::string s;
    while (getline(fin, s)) {
        stringstream ss(s);
        std::vector<cell> v;
        while (getline(ss, s, delim)) {
            v.push_back(any(&s, "string"));
        }
        tb.push_back(v);
    }
    fin.close();
    return 0;
}
int table::write(std::string &path, char delim = ',') {
    fstream fout(path);
    for (auto v : tb) {
        int sz = v.size();
        for (int i = 0; i < sz; i++) {
            fout << v[i].val;
            if (i + 1 == sz) break;
            fout << delim;
        }
        fout << '\n';
    }
    fout.close();
    return 0;
}
date::date(std::string str)
{
    std::vector<std::string> temp = split(str, "-");
    year = stoi(temp[0]);
    month = stoi(temp[1]);
    day = stoi(temp[2]);
}

time::time(std::string str)
{
    std::vector<std::string> temp = split(str, ":");
    hour = stoi(temp[0]);
    min = stoi(temp[1]);
    sec = stoi(temp[2]);
}