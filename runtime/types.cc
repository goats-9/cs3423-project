#include "types.hh"
#include "any.hh"
#include "helper.hh"
#include "state.hh"
using namespace std;

extern state st;

void cell::destroy()
{
    // Nothiing
}
void cell::construct(const cell &a)
{
    val = a.val;
}
void cell::construct(const any &a)
{
    val = a;
}
// int table::read(std::string &path, char delim = ',') {
//     fstream fin(path, std::ios_base::in);
//     if (!fin) return -1;
//     // CSV handler: Read file line by line
//     std::string s;
//     int row = 0;
//     while (getline(fin, s)) {
//         int col = 0;
//         stringstream ss(s);
//         std::vector<cell> v;
//         while (getline(ss, s, delim)) {
//             tb[{row, col}] = cell(any(&s, "string"));
//             col++;
//         }
//     }
//     fin.close();
//     return 0;
// }
// int table::write(std::string &path, char delim = ',') {
//     std::fstream fout(path);
//     std::vector<std::vector<any>> tb_vec;
//     for (auto v : tb) {
//         tb_vec[v.first.first][v.first.second] = any(*v.second.val);
//     }
//     for (auto row : tb_vec) {
//         for (auto cell : row) {
//             fout << cell;
//             if (cell != row.back()) fout << ',';
//         }
//         fout << "\n";
//     }
//     fout.close();
//     return 0;
// }
// any table::operator[] (any &dim)
// {
//     if (dim.type != "shape") throw std::runtime_error("Improper usage of table access.");
//     shape dim_data = *(shape *)dim.data;
//     return tb[{dim_data.first, dim_data.second}];
// }

range::range(std::string str, const pos &p)
{
    st.infunc(p);
    std::vector<std::string> temp = split(str, "~");
    step = stoi(temp[1]);
    temp = split(temp[0], ":");
    start = stoi(temp[0]);
    stop = stoi(temp[1]);
    if (step <= 0)
    {
        throw runtime_error("step of range cannot be negetive or zero");
    }
    if (start < 0)
    {
        throw runtime_error("start of range cannot be negetive");
    }
    if (stop < 0)
    {
        throw runtime_error("stop of range cannot be negetive");
    }
    if (start > stop)
    {
        throw runtime_error("stop of range cannot be less than start of range cannot be negetive");
    }
    st.outfunc();
}

date::date(std::string str, const pos &p)
{
    st.infunc(p);
    std::vector<std::string> temp = split(str, "-");
    year = stoi(temp[0]);
    month = stoi(temp[1]);
    day = stoi(temp[2]);
    if (year < 0)
    {
        throw runtime_error("year cannot be negetive");
    }
    if (month < 0)
    {
        throw runtime_error("month cannot be negetive");
    }
    if (day < 0)
    {
        throw runtime_error("day cannot be negetive");
    }
    st.outfunc();
}

Time::Time(std::string str, const pos &p)
{
    st.infunc(p);
    std::vector<std::string> temp = split(str, ":");
    hour = stoi(temp[0]);
    min = stoi(temp[1]);
    sec = stoi(temp[2]);
    if (hour < 0 || hour > 24)
    {
        throw runtime_error("hour cannot be negetive or more than 24");
    }
    if (min < 0 || min > 60)
    {
        throw runtime_error("min cannot be negetive or more than 60");
    }
    if (sec < 0 || sec > 60)
    {
        throw runtime_error("sec cannot be negetive or more than 60");
    }
    st.outfunc();
}

shape::shape(const any &a, const any &b, const pos &p)
{
    st.infunc(p);
    if ((a.type != "int" && a.type != "range") || (b.type != "int" && b.type != "range"))
    {
        throw runtime_error("both elements of shape should be (int OR range,int OR range) but found (" + a.type + "," + b.type + ")");
    }
    if ((a.type == "int" && *(int *)a.data < 0) || (b.type == "int" && *(int *)b.data < 0))
    {
        throw runtime_error("both elements have to be positive");
    }

    vals = {a, b};
    st.outfunc();
}