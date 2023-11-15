#include "/home/anshul/programming/Compiler-2/project/runtime/include/runtime.hh"
state st("../test/sem.tblt");
class myclass2{
public:
funcMap<myclass2> func;
funcParams func_params;
memMap mem;
myclass2(const pos &p)
{
st.infunc(p);
func_decl();
st.outfunc();
}
myclass2(any a,any b,const pos &p)
{
st.infunc(p);
func_decl();
mem["a"] = a;
mem["b"] = b;
st.outfunc();
}
any test3(vector<any> params,const pos &p)
{
st.infunc(p);
SUB(params[0],mem["c"],pos(12,9,12,22,"SUB"));
st.outfunc();
return any();
}
any test20(vector<any> params,const pos &p)
{
st.infunc(p);
any ret_val = SUB(params[0],params[1],pos(16,16,16,24,"SUB"));
st.outfunc();
return ret_val;
st.outfunc();
return any();
}
void func_decl(){
func["test3"] = &myclass2::test3;
func_params["test3"] = 1;
func["test20"] = &myclass2::test20;
func_params["test20"] = 2;
}
myclass2(const myclass2 &a){
copyConstruct
}
};
class myclass1{
public:
funcMap<myclass1> func;
funcParams func_params;
memMap mem;
void func_decl(){
}
myclass1(const pos &p){
func_decl();
}
myclass1(const myclass1 &a){
copyConstruct
}
};
any fib(any n,const pos &p)
{
st.infunc(p);
if (to_bool(EQ(n,any(new int(0),"int"),pos(26,9,26,15,"EQ")),pos(26,5,26,16,"if predicate")))
{
any ret_val = any(new int(1),"int");
st.outfunc();
return ret_val;
}
any dp = any(new vector<any>({any(new int(1),"int"),any(new int(1),"int"),any(new int(2),"int")}),"array");
any ret_val = dp.at(n,pos(31,14,31,17,"[] operator"));
st.outfunc();
return ret_val;
st.outfunc();
return any();
}
any printArray(any arr,const pos &p)
{
st.infunc(p);
any i = SUB(arr.run("size",{},pos(36,17,36,27,"size")),any(new int(1),"int"),pos(36,13,36,30,"SUB"));
while (to_bool(GTE(i,any(new int(0),"int"),pos(37,11,37,17,"GTE")),pos(37,18,37,18,"while predicate"))){
DISP(arr.at(i,pos(39,17,39,20,"[] operator")),pos(39,9,39,21,"DISP"));
DISP(TYPEOF((arr.at(i,pos(40,24,40,27,"[] operator"))),pos(40,14,40,28,"TYPEOF")),pos(40,9,40,29,"DISP"));
i = SUB(i,any(new int(1),"int"),pos(41,13,41,21,"SUB"));
}
st.outfunc();
return any();
}
int main(int agrc, char* argv[])
{
try
{
DISP(fib(any(new int(3),"int"),pos(46,10,46,16,"fib")),pos(46,5,46,17,"DISP"));
return to_int(any(new int(0),"int"),pos(47,5,47,14,"return in function main"));
}
catch(const runtime_error &e){
disp_error(e);
return 1;
}
return 0;
}void any::Constructor(const any &a)
{
if (a.type == "myclass2"){
data = new myclass2(*(myclass2 *)a.data);
type = "myclass2";
}
if (a.type == "myclass1"){
data = new myclass1(*(myclass1 *)a.data);
type = "myclass1";
}
}
void any::Destructor(){
if (type == "myclass2"){
delete (myclass2*)data;
}
if (type == "myclass1"){
delete (myclass1*)data;
}
}
any &any::Accessor(const string &id){
if (type == "myclass2"){
myclass2 *ptr = (myclass2 *)data;
return ptr->mem[id];
}
if (type == "myclass1"){
myclass1 *ptr = (myclass1 *)data;
return ptr->mem[id];
}
throw uni_err("dot(.)",*this);}
any any::Runner(const string &id,const vector<any> &params, const pos &p){
if (type == "myclass2"){
any (myclass2::*f)(vector<any>, const pos &);
myclass2 *ptr = (myclass2 *)data;
runnerCode
}
if (type == "myclass1"){
any (myclass1::*f)(vector<any>, const pos &);
myclass1 *ptr = (myclass1 *)data;
runnerCode
}
throw uni_err("dot(.)",*this);
}
