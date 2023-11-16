# cs3423-project

## Instruction to run:
```shell
mkdir build
cd build
cmake ..
cmake --build . --target install
./tabulate /path/to/source.tblt
./a.out
```
## Translation

### Handling constants

| Tabulate | C++ |
|-|-|
|`const` | `any(new const_data_type(value), "const_data")` |
|`2` | `(new int(2),"int")` |
|`"hello"` | `any(new string("hello"),"string")` |
|`new custom_class()` | `any(new custom_class(),"custom_class")` |

### Declarations and Assignments

| Tabulate | C++ |
|-|-|
|`let a;` | `any a;` |
|`const a;` | `const any a;` |
|`const a = 90;` | `const any a = any(new int(90),"int");` |

### Function Calls

| Tabulate | C++ |
|-|-|
| `func(a,2)` | `func(a,any(new int(2),"int"),pos(start_line,start_col,end_line,end_col,"func"))` | 
| `fun main()` | `int main(int argc, char* argv[])` | 

### Structs

| Tabulate | C++ |
|-|-|
| `a.func(c,d)` | `a.run("func",{c,d},pos(start_line,start_col,end_line,end_col,"func"))` |
| `b.d` | `a.access("d")` |

5. defining struct

<table>
<tr>
<th>Tabulate</th>
<th>C++</th>
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
