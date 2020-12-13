Code embedded in `<>` is run-time implicit as well as scope `::` sign.

```
func foo() {
    var kek = 1
    
    func bar() {
        println(kek)
    }

    bar()
}

foo()
```

```
func foo() {
    var kek = 1
    
    foo::bar()<kek>
}

func foo::bar() <kek> {
    println(kek)
}

foo()
```

```
DefineGlobal 0 (string constant 'foo')
-> Enter function 'foo' scope and compile to its chunk
IntConst 0 (integer constant 1)
StoreLocal 0 (store integer constant 1 in local variable 'kek')
*Compile-time: local function 'bar' declared*
-> Enter function 'bar' scope and compile to its chunk
LoadFunc 0 (load 'println' from function stack, make closure by making needed upvalue-references and push it to the stack)
Invoke 0 (invoke 'println' without any arguments)
<- Exit function 'bar'
LoadFunc 2 (load 'foo::bar' from function stack)
Invoke 0 1 
<- Exit function 'foo'

LoadFunc 1 (load 'foo' from function stack)
Invoke 0 (invoke 'foo' without any arguments)
```

### LoadFunc
```
1. Get function by offset from function stack
2. Make closure from found function
3. Push closure to the stack
```

#### Function
Function is distributive object.
```c++
struct Function {
    uint32_t name_offset; // Offset to function name (string in constant pool)
    std::vector<uint32_t> upvalues; // List of parent function locals listed by their offset
    byte_list code; // Function body bytecode
}
```

#### Closure
Closure is run-time object.
```c++
struct Closure {
    std::shared_ptr<Function> function;
    
};
```
