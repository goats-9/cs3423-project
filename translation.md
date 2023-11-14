# Translation

1. handling constants
```
const -> any(new const_data_type(value), "const_data")
2 -> any(new int(2),"int")
"hello" -> any(new string("hello"),"string")
new custom_class() -> any(new custom_class(),"custom_class")
```

2. handling declaration and assignments
```
let a; -> any a;
const a; -> const any a;
const a = 90; -> const any a = any(new int(90),"int");
```

3. handling function call
```
func(a,2) -> func(a,any(new int(2),"int"),pos(start_line,start_col,end_line,end_col,"func")) 
fun main() -> int main(int argc, char* argv[]) 
```

4. handling struct
```
a.func(c,d) -> a.run("func",{c,d},pos(start_line,start_col,end_line,end_col,"func"))
b.d -> a.access("d")
```

5. defining struct
<table>
<tr>
<th>source code</th>
<th>translation</th>
</tr>
<tr>
<td>

```
struct test
{
    fun func1(a)
    {
        return "hello";
    }
    fun func2()
    {
        return 1;
    }
};
```
</td>
<td>

```c++
class test
{
public:
    Function_map<test> func_map;
    Function_params_map params_map;
    any func1(vector<any> params)
    {
        return any(new string("hello"),"string");
    }
    any func1(vector<any> params)
    {
        return any(new int(1),"int");
    }
    void func_decl()
    {
        func_map["func1"] = &test::func1;
        param_map["func1"] = 1;
        func_map["func2"] = &test::func2;
        param_map["func2"] = 0;
    }
};
```
</td>
</tr>
</table>
