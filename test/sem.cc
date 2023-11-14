#include "/home/anshul/programming/Compiler-2/project/runtime/include/runtime.hh"
state st("../test/sem.tblt");
any test(const pos &p)
{
st.infunc(p);
any ret_val = any(new int(1),"int");
st.outfunc();
return ret_val;
st.outfunc();
return any();
}
any test2(const pos &p)
{
st.infunc(p);
any ret_val = any(new int(34),"int");
st.outfunc();
return ret_val;
st.outfunc();
return any();
}
any test3(any e,any f,const pos &p)
{
st.infunc(p);
any ret_val = ADD(e,f,pos(24,12,24,20,"ADD"));
st.outfunc();
return ret_val;
st.outfunc();
return any();
}
any printArray(any arr,const pos &p)
{
st.infunc(p);
any i = SUB(arr.run("size",{},pos(29,17,29,27,"size")),any(new int(1),"int"),pos(29,13,29,30,"SUB"));
while (to_bool(GTE(i,any(new int(0),"int"),pos(30,11,30,17,"GTE")),pos(30,18,30,18,"while predicate"))){
DISP(arr.at(i,pos(32,17,32,20,"[] operator")),pos(32,9,32,21,"DISP"));
DISP(TYPEOF((arr.at(i,pos(33,24,33,27,"[] operator"))),pos(33,14,33,28,"TYPEOF")),pos(33,9,33,29,"DISP"));
i = SUB(i,any(new int(1),"int"),pos(34,13,34,21,"SUB"));
}
st.outfunc();
return any();
}
int main(int agrc, char* argv[])
{
try
{
any e = test(pos(43,13,43,19,"test"));
any f = test2(pos(44,13,44,20,"test2"));
any d = test3(e,f,pos(45,13,45,23,"test3"));
DISP(any(new string("d is "),"string"),pos(50,5,50,18,"DISP"));
DISP(d,pos(51,5,51,12,"DISP"));
while (to_bool(NE(d,any(new int(36),"int"),pos(53,11,53,18,"NE")),pos(53,19,53,19,"while predicate"))){
d = ADD(d,any(new int(1),"int"),pos(55,13,55,21,"ADD"));
DISP(any(new string("d is "),"string"),pos(56,9,56,22,"DISP"));
DISP(d,pos(57,9,57,16,"DISP"));
}
any arr = any(new vector<any>({any(new int(1),"int"),any(new string("hello"),"string"),any(new double(90.8),"double"),any(new bool(false),"bool")}),"array");
DISP(any(new string("Displaying array using a function"),"string"),pos(62,5,62,46,"DISP"));
printArray(arr,pos(63,5,63,20,"printArray"));
return to_int(any(new bool(true),"bool"),pos(64,5,64,17,"return in function main"));
}
catch(const runtime_error &e){
disp_error(e);
return 1;
}
return 0;
}