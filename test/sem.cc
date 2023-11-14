#include "/home/anshul/programming/Compiler-2/project/runtime/include/runtime.hh"
state st("../test/sem.tblt");
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
any ret_val = any(new string("34"),"string");
st.outfunc();
return ret_val;
}
any test3(any e,any f,const pos &p)
{
st.infunc(p);
any ret_val = ADD(e,f,pos(24,12,24,20,"ADD"));
st.outfunc();
return ret_val;
}
int main(int agrc, char* argv[])
{
try
{
any e = test(pos(32,13,32,19,"test"));
any f = test2(pos(33,13,33,20,"test2"));
const any d = test3(e,f,pos(34,15,34,25,"test3"));
return to_int(any(new int(0),"int"),pos(39,5,39,14,"return"));
}
catch(const runtime_error &e){
disp_error(e);
return 1;
}
return 0;
}