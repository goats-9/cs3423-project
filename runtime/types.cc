#include "types.hh"
#include "any.hh"
#include "helper.hh"
#include "state.hh"
using namespace std;

extern state st;

void cell::construct(const cell &a)
{
    val = a.val;
}
void cell::construct(const any &a)
{
    if (isInbuilt(a.type)) val = a;
    else throw runtime_error("cell cannot take non-primitive datatype.");
}
any cell::get() {
    return val;
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
    std::fstream fout(path, std::ios_base::out);
    if (!fout)
    {
        throw runtime_error("cannot open file " + path);
    }
    std::vector<std::vector<any>> tb_vec(max_row, std::vector<any>(max_col));
    for (auto v : tb)
    {
        tb_vec[v.first.first][v.first.second] = any(v.second.val);
    }
    for (int i = 0; i < max_row; i++)
    {
        for (int j = 0; j < max_col; j++)
        {
            fout << tb_vec[i][j];
            if (j + 1 != max_col) {
                fout << delim;
            }
        }
        fout << "\n";
    }
    fout.close();
    return any();
}
any table::assign(const any &dim, const any &elements)
{
    if (dim.type != "shape") throw runtime_error("invalid table access");
    shape dshape = *(shape *)dim.data;
    bool cond = (dshape.vals.first.type == "int" || dshape.vals.first.type == "range")
                && (dshape.vals.second.type == "int" || dshape.vals.second.type == "range");
    if (!cond) throw runtime_error("invalid dimensions entered");
    range r1(dshape.vals.first), r2(dshape.vals.second);
    if (elements.type != "array") throw runtime_error("2D array not entered");
    std::vector<any> elements_arr = *(std::vector<any> *)elements.data;
    int l1 = (r1.stop - r1.start + 1)/r1.step + (r1.stop - r1.start + 1)%r1.step;
    if (l1 != (int)elements_arr.size()) throw runtime_error("First dimension mismatch");
    for (int i = r1.start, j = 0; i <= r1.stop; i += r1.step, j++) {
        if (elements_arr[j].type != "array") throw runtime_error("2D array not entered");
        std::vector<any> elements_arr_arr = *(std::vector<any> *)elements_arr[j].data;
        int l2 = (r2.stop - r2.start + 1)/r2.step + (r2.stop - r2.start + 1)%r2.step;
        if (l2 != (int)elements_arr_arr.size()) throw runtime_error("Dimension mismatch");
        for (int ii = r2.start, jj = 0; ii <= r2.stop; ii += r2.step, jj++) {
            if (!isPrimitive(elements_arr_arr[jj].type)) throw runtime_error("Element is not a primitive");
            tb[{i,ii}] = cell(elements_arr_arr[jj]);
            max_col = max(max_col, ii+1);
        }
        max_row = max(max_row, i+1);
    }
    return any();
}

any table::dim()
{
    any _row = any(new int(max_row), "int");
    any _col = any(new int(max_col), "int");
    return any(new shape(_row, _col), "shape");
}

any table::get(const any &dim)
{
    if (dim.type != "shape") throw runtime_error("invalid table access");
    shape dshape = *(shape *)dim.data;
    bool cond = (dshape.vals.first.type == "int" || dshape.vals.first.type == "range")
                && (dshape.vals.second.type == "int" || dshape.vals.second.type == "range");
    if (!cond) throw runtime_error("invalid dimensions entered");
    range r1(dshape.vals.first), r2(dshape.vals.second);
    std::vector<any> ret;
    for (int i = r1.start; i <= r1.stop; i += r1.step) {
        std::vector<any> ret_push;
        for (int j = r2.start; j <= r2.stop; j += r2.step) {
            if (tb.find({i,j}) == tb.end()) ret_push.push_back(any());
            else ret_push.push_back(any(&tb[{i,j}], "cell"));
        }
        ret.push_back(any(new std::vector<any>(ret_push), "array"));
    }
    return any(new std::vector<any>(ret), "array");
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

range::range(const any &a) {
    if (a.type == "int") {
        int x = *(int *)a.data;
        start = x, stop = x, step = 1;
    } else if (a.type == "range") {
        range r = *(range *)a.data;
        start = r.start, stop = r.stop, step = r.step;
    } else throw runtime_error("cannot cast object to range");
}

Time::Time(std::string str, const pos &p)
{
    st.infunc(p);
    std::vector<std::string> temp = split(str, ":");
    hour = stoi(temp[0]);
    min = stoi(temp[1]);
    sec = stoi(temp[2]);
    if (hour < 0 || hour >= 24)
    {
        throw runtime_error("hour must be between 0 and 23");
    }
    if (min < 0 || min >= 60)
    {
        throw runtime_error("min must be between 0 and 59");
    }
    if (sec < 0 || sec >= 60)
    {
        throw runtime_error("sec must be between 0 and 59");
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