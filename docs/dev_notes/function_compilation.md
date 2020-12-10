## Raw

```
func foo(arg: string): unit {
    println(arg)
}

foo('kek')
```

### Byte-Code
```
LoadGlobal 0 (String constant 'foo' -> function)
StringConst 2
Invoke 0 ('foo("kek")')

Constant pool:
0 -> {
    Type: String
    Value: 'foo'
},
1 -> {
    Type: String
    Value: 'println'
}
2 -> {
    Type: String
    Value: 'kek'
}

Function stack:
0 -> {
    name_offset: 0 (String constant 'foo')
    Code:
        LoadGlobal 1 (String constant 'println' -> global native_func 'println')
        LoadLocal 0 (arg)        
        Invoke 0 ('println(arg)')
}
```

### Recursion
```
func foo(arg: int): unit {
    if arg == 10 {
        println('kek')
    } else {
        foo(arg + 1)
    }
}

foo(0)
```

### Byte-Code
```
-- Byte-code --
LoadGlobal 0 (String constant 'foo' -> function)

StringConst 2
Invoke 0 ('foo("kek")')

Constant pool:
0 -> {
    Type: String
    Value: 'foo'
},
1 -> {
    Type: String
    Value: 'println'
}
2 -> {
    Type: String
    Value: 'kek'
}

Function stack:
0 -> {
    name_offset: 0 (String constant 'foo')
    Code:
        LoadGlobal 1 (String constant 'println' -> global native_func 'println')
        LoadLocal 0 (arg)
        Invoke 0 ('println(arg)')
}
```
