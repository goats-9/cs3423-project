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
int table::read(any &path, any &delim) {
    if (path.type != "string" || delim.type != "string") 
    {
        bi_err("READ", path, delim);
    }
    std::string _path = *(std::string *)path.data;
    std::string _delim = *(std::string *)delim.data;
    fstream fin(_path, std::ios_base::in);
    if (!fin) return -1; 
    // CSV handler: Read file line by line
    std::string s;
    int row = 0;
    while (getline(fin, s)) {
        int col = 0;
        stringstream ss(s);
        std::vector<cell> v;
        // Take only first char to use as delimiter
        while (getline(ss, s, _delim[0])) {
            tb[{row, col}] = cell(any(&s, "string"));
            col++;
        }
    }
    fin.close();
    return 0;
}
int table::write(any &path, any &delim) {
    if (path.type != "string" || delim.type != "string") 
    {
        bi_err("READ", path, delim);
    }
    std::string _path = *(std::string *)path.data;
    std::string _delim = *(std::string *)delim.data;
    std::fstream fout(_path);
    std::vector<std::vector<any>> tb_vec;
    for (auto v : tb) {
        tb_vec[v.first.first][v.first.second] = any(*v.second.val);
    }
    for (auto row : tb_vec) {
        for (auto cell : row) {
            fout << cell;
            if (cell != row.back()) fout << _delim;
        }
        fout << "\n";
    }
    fout.close();
    return 0;
}
any table::operator[] (any &dim)
{
    if (dim.type != "shape") throw std::runtime_error("Improper usage of table access.");
    shape dim_data = *(shape *)dim.data;
    return any(&tb[dim_data.vals], "cell");
}
any table::operator= (any &aval)
{

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