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
    int row = 0;
    while (getline(fin, s)) {
        int col = 0;
        stringstream ss(s);
        std::vector<cell> v;
        while (getline(ss, s, delim)) {
            tb[{row, col}] = cell(any(&s, "string"));
            col++;
        }
    }
    fin.close();
    return 0;
}
int table::write(std::string &path, char delim = ',') {
    std::fstream fout(path);
    std::vector<std::vector<any>> tb_vec;
    for (auto v : tb) {
        tb_vec[v.first.first][v.first.second] = any(*v.second.val);
    }
    for (auto row : tb_vec) {
        for (auto cell : row) {
            fout << cell;
            if (cell != row.back()) fout << ',';
        }
        fout << "\n";
    }
    fout.close();
    return 0;
}
any table::sum(any &r1, any &r2) {
    if(r1.type == "range" && r2.type == "range"){
        double total = 0;
        for (int i = r1.data.start; i <= r1.data.end; i += r1.data.step) {
            for (int j = r2.data.start; j <= r2.data.end; j += r2.data.step) {
                auto it = tb.find({i, j});
                if (it != tb.end()) {
                    total += it->second.getDoubleVal();
                }
            }
        }
        // return table / cell? debate?
    }
    else{
        runtime_error e = std::runtime_error("Argument must be a range.");
        disp_error(e);
    }
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