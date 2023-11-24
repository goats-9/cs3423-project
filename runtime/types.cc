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
any table::read(const any &_path, const any &_delim = any(new string(","), "string"))
{
    if (_path.type != "string")
    {
        throw runtime_error("path of file should be string but found " + _path.type);
    }
    if (_delim.type != "string")
    {
        throw runtime_error("deliminiter should be string but found " + _delim.type);
    }
    std::string path = *(string *)_path.data;
    std::string __delim = *(string *)_delim.data;
    if (__delim.size() != 1)
    {
        throw runtime_error("deliminiter should be 1 character long only");
    }
    char delim = __delim[0];
    fstream fin(path, std::ios_base::in);
    if (!fin)
    {
        throw runtime_error("cannot open file " + path);
    }
    // CSV handler: Read file line by line
    std::string s;
    int row = 0;
    while (getline(fin, s))
    {
        int col = 0;
        stringstream ss(s);
        std::vector<cell> v;
        while (getline(ss, s, delim))
        {
            tb[{row, col}] = cell(any(&s, "string"));
            col++;
        }
        max_col = max(col, max_col);
        row++;
    }
    fin.close();
    max_row = row;
    return any();
}
any table::write(const any &_path, const any &_delim = any(new string(","), "string"))
{
    if (_path.type != "string")
    {
        throw runtime_error("path of file should be string but found " + _path.type);
    }
    if (_delim.type != "string")
    {
        throw runtime_error("deliminiter should be string but found " + _delim.type);
    }
    std::string path = *(string *)_path.data;
    std::string __delim = *(string *)_delim.data;
    if (__delim.size() != 1)
    {
        throw runtime_error("deliminiter should be 1 character long only");
    }
    char delim = __delim[0];
    std::fstream fout(path);
    if (!fout)
    {
        throw runtime_error("cannot open file " + path);
    }
    std::vector<std::vector<any>> tb_vec;
    for (auto v : tb)
    {
        tb_vec[v.first.first][v.first.second] = any(v.second.val);
    }
    for (auto row : tb_vec)
    {
        for (auto cell : row)
        {
            fout << cell;
            if (cell != row.back())
                fout << delim;
        }
        fout << "\n";
    }
    fout.close();
    return any();
}
any table::assign(const any &sp, const any &elements)
{
    if (sp.type != "shape")
    {
        throw runtime_error("Data type of shape should be shape but found " + sp.type);
    }
    any first = ((shape *)sp.data)->vals.first;
    any second = ((shape *)sp.data)->vals.second;
    if (first.type == "int" && second.type == "int")
    {

    }
}

any table::dim()
{
    any _row = any(new int(max_row), "int");
    any _col = any(new int(max_col), "int");
    return any(new shape(_row, _col), "shape");
}

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
shape::shape(const any &a, const any &b)
{
    vals = {a, b};
}