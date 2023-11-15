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
mem["a"] = SUB(params[0],mem["b"],pos(12,18,12,31,"SUB"));
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
any dp = any(new vector<any>({any(new int(1),"int"),any(new int(1),"int")}),"array");
any i = any(new int(2),"int");
while (to_bool(LT(i,n,pos(33,12,33,17,"LT")),pos(33,18,33,18,"while predicate"))){
dp.run("add_item",{ADD(dp.at(SUB(i,any(new int(1),"int"),pos(35,28,35,36,"SUB")),pos(35,27,35,37,"[] operator")),dp.at(SUB(i,any(new int(2),"int"),pos(35,41,35,49,"SUB")),pos(35,40,35,50,"[] operator")),pos(35,21,35,51,"ADD"))},pos(35,9,35,52,"add_item"));
i = ADD(i,any(new int(1),"int"),pos(36,13,36,21,"ADD"));
}
any ret_val = dp.at(SUB(n,any(new int(1),"int"),pos(39,15,39,23,"SUB")),pos(39,14,39,24,"[] operator"));
st.outfunc();
return ret_val;
st.outfunc();
return any();
}
any printArray(any arr,const pos &p)
{
st.infunc(p);
any i = any(new int(0),"int");
DISP(any(new string("printing array:"),"string"),pos(45,5,45,28,"DISP"));
while (to_bool(LTE(i,SUB(arr.run("size",{},pos(46,20,46,30,"size")),any(new int(1),"int"),pos(46,16,46,33,"SUB")),pos(46,11,46,33,"LTE")),pos(46,34,46,34,"while predicate"))){
DISP(arr.at(i,pos(48,17,48,20,"[] operator")),pos(48,9,48,21,"DISP"));
i = ADD(i,any(new int(1),"int"),pos(49,13,49,21,"ADD"));
}
st.outfunc();
return any();
}
int main(int agrc, char* argv[])
{
try
{
DISP(fib(any(new int(5),"int"),pos(54,10,54,16,"fib")),pos(54,5,54,17,"DISP"));
any obj2 = any(new myclass2(any(new int(1),"int"),any(new int(2),"int"),pos(56,16,56,33,"new myclass2")),"myclass2");
obj2.run("test3",{any(new int(56),"int")},pos(58,5,58,19,"test3"));
DISP(obj2.access("a",pos(59,10,59,16,"dot(.)")),pos(59,5,59,17,"DISP"));
any obj1 = any(new myclass1(pos(61,16,61,30,"new myclass1")),"myclass1");
obj1.access("arr",pos(63,5,63,13,"dot(.)")) = any(new vector<any>({any(new int(1),"int"),any(new vector<any>({any(new string("2.0ef"),"string"),any(new string("yuy"),"string"),any(new int(3),"int")}),"array"),any(new double(3.932),"double")}),"array");
printArray(obj1.access("arr",pos(64,16,64,24,"dot(.)")),pos(64,5,64,25,"printArray"));
return to_int(any(new int(0),"int"),pos(65,5,65,14,"return in function main"));
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
