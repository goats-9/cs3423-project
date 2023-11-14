#include "/home/anshul/programming/Compiler-2/project/runtime/include/runtime.hh"
state st("../test/sem.tblt");
class test
{
public:
    funcMap<test> func;
    funcParams func_params;
    memMap mem;
    test()
    {
        func_decl();
    }
    test(any a, any b)
    {
        func_decl();
        mem["a"] = a;
        mem["b"] = b;
    }
    any test3(vector<any> params)
    {
        SUB(params[0], mem["c"], pos(12, 9, 12, 22, "SUB"));
        return any();
    }
    any test20(vector<any> params)
    {
        return SUB(params[0], params[1], pos(16, 16, 16, 24, "SUB"));
        return any();
    }
    void func_decl()
    {
        func["test3"] = &test::test3;
        func_params["test3"] = 1;
        func["test20"] = &test::test20;
        func_params["test20"] = 2;
    }
    test(const test &a)
    {
        copyConstruct
    }
};
class myclass
{
public:
    funcMap<myclass> func;
    funcParams func_params;
    memMap mem;
    void func_decl()
    {
    }
    myclass()
    {
        func_decl();
    }
    myclass(const myclass &a)
    {
        copyConstruct
    }
};
any test(const pos &p)
{
    st.infunc(p);
    any ret_val = any(new int(1), "int");
    st.outfunc();
    return ret_val;
    st.outfunc();
    return any();
}
any test2(const pos &p)
{
    st.infunc(p);
    any ret_val = any(new int(34), "int");
    st.outfunc();
    return ret_val;
    st.outfunc();
    return any();
}
any test3(any e, any f, const pos &p)
{
    st.infunc(p);
    any ret_val = ADD(e, f, pos(35, 12, 35, 20, "ADD"));
    st.outfunc();
    return ret_val;
    st.outfunc();
    return any();
}
any printArray(any arr, const pos &p)
{
    st.infunc(p);
    any i = SUB(arr.run("size", {}, pos(40, 17, 40, 27, "size")), any(new int(1), "int"), pos(40, 13, 40, 30, "SUB"));
    while (to_bool(GTE(i, any(new int(0), "int"), pos(41, 11, 41, 17, "GTE")), pos(41, 18, 41, 18, "while predicate")))
    {
        DISP(arr.at(i, pos(43, 17, 43, 20, "[] operator")), pos(43, 9, 43, 21, "DISP"));
        DISP(TYPEOF((arr.at(i, pos(44, 24, 44, 27, "[] operator"))), pos(44, 14, 44, 28, "TYPEOF")), pos(44, 9, 44, 29, "DISP"));
        i = SUB(i, any(new int(1), "int"), pos(45, 13, 45, 21, "SUB"));
    }
    st.outfunc();
    return any();
}
int main(int agrc, char *argv[])
{
    try
    {
        any e = test(pos(54, 13, 54, 19, "test"));
        any f = test2(pos(55, 13, 55, 20, "test2"));
        any d = test3(e, f, pos(56, 13, 56, 23, "test3"));
        DISP(any(new string("d is "), "string"), pos(61, 5, 61, 18, "DISP"));
        DISP(d, pos(62, 5, 62, 12, "DISP"));
        while (to_bool(NE(d, any(new int(36), "int"), pos(64, 11, 64, 18, "NE")), pos(64, 19, 64, 19, "while predicate")))
        {
            d = ADD(d, any(new int(1), "int"), pos(66, 13, 66, 21, "ADD"));
            DISP(any(new string("d is "), "string"), pos(67, 9, 67, 22, "DISP"));
            DISP(d, pos(68, 9, 68, 16, "DISP"));
        }
        any arr = any(new vector<any>({any(new int(1), "int"), any(new string("hello"), "string"), any(new double(90.8), "double"), any(new bool(false), "bool")}), "array");
        DISP(any(new string("Displaying array using a function"), "string"), pos(73, 5, 73, 46, "DISP"));
        printArray(arr, pos(74, 5, 74, 20, "printArray"));
        return to_int(any(new bool(true), "bool"), pos(75, 5, 75, 17, "return in function main"));
    }
    catch (const runtime_error &e)
    {
        disp_error(e);
        return 1;
    }
    return 0;
}