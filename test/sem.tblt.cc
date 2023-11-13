#include "/home/anshul/programming/Compiler-2/project/runtime/include/runtime.hh"
{
}
{
mem["a"] = a;
mem["b"] = b;
}
{
any a = any(new test(),"test");
any b = any(new test(any(new int(1),"int"),any(new int(2),"int")),"test");
any c = a.access("att1").at(any(new int(0),"int")).at(any(new int(1),"int")).at(any(new int(2),"int")).run("att2",{},pos(14,13,14,35,"att2")).at(any(new int(0),"int")).run("att6",{a,b},pos(14,13,14,48,"att6"));
(AND(SUB(ADD(a.access("att1").at(any(new int(0),"int")).at(any(new int(9),"int")).at(any(new range("1:4~2"),"range")).run("att2",{},pos(15,14,15,40,"att2")).access("att6"),a.access("attr2"),pos(15,10,15,54,"ADD")).run("run",{},pos(15,10,15,60,"run")),c.access("attr2"),pos(15,6,15,69,"SUB")).access("att1").at(any(new int(0),"int")),any(new int(1),"int"),pos(15,6,15,83,"AND"))).run("run",{},pos(15,5,15,90,"run"));
return to_int(any(new int(0),"int"),pos(16,5,16,14,"return"));
}
