#include "/home/anshul/programming/Compiler-2/project/runtime/include/runtime.hh"
any test(const pos &p)
{
st.infunc(p);
any ret_val = any(new int(1),"int");
st.outfunc();
return ret_val;
}
any test2(const pos &p)
{
st.infunc(p);
any ret_val = any(new int(34),"int");
st.outfunc();
return ret_val;
}
int main(int agrc, char* argv[])
{
try
{
any e = test(pos(14,13,14,19,"test"));
any f = test2(pos(15,13,15,20,"test2"));
const any d = ADD(e,f,pos(16,15,16,23,"ADD"));
return to_int(any(new int(0),"int"),pos(18,5,18,14,"return"));
}
catch(const runtime_error &e){
disp_error(e);
return 1;
}
return 0;
}