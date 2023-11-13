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
```

4. handling struct
```
a.func(c,d) -> a.run("func",{c,d},pos(start_line,start_col,end_line,end_col,"func"))
b.d -> a.access("d")
```