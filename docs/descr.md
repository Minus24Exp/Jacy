The main rule I want to stick to while developing:
"Not so boring as Python, not so ambiguous as Ruby"
I hope it's possible...

## Architecture
- Everything is an Object, primitives are objects too and all operators implemented as magic functions
- The compiler always inside a function. The top-level code is always in some kinda `main()` function

- At first, I wanted to make OOP same to C++, where instance fields and static fields won't require `this` keyword. But I rewrote Object structure and made it cleaner, so now `this` keyword is required.

### Control flow

#### `if`/`elif`(`else if`)/`else`
In Yocto `if` is an expression that sets current value to last evaluated in its block.
So, it's possible to use `if` in assignment or as ternary:
```
// Assignment
var a = if() //...

var tern = if(1) true else false
// or without parentheses
var tern = if 1 => true else false
```

`elif` is syntax sugar that automatically preprocessing to `else if` on lexing level.

`if`/`elif`/`else` may have one-line body:
```
if(a) print(a)
else print('nope')
```

`if`/`elif` may have conditions without parentheses:
```
if a
    print(a)
elif b
    print(b)
else
    print('nope')
```
But, what's important, then there MUST be new-line after condition.
Or, it could be written as one-line body if put braces:
```
if a { print(a) }
elif b { print(b) }
else { print('nope') }
// or
if a => print(a)
elif b => print(b)
else print('nope')
// or
if a => { print(a) }
elif b => { print(b) }
else { print('nope') }
```

So, all control flow statements may have one-line block if:
1. Block is in braces
2. Block is on the next line (`else` does not require it)
3. Block has `=>` before (in this case it's possible to put block on the same line)

Important note: As `else` does not absolutely require `=>` for one-line blocks, it cannot have it,
it means that `else => print(something)` will cause error.

Important note: As `elif` converts to `else if` at lexing level then all errors about it will
reference to `else`. Maybe I'll solve it in the future.

#### `while` statement
`while` works the same as `while` in other languages.
It also have the same rules for one-line blocks as `if` (watch `if` section).

All examples are equal:
```
while(a) print(a)
// or
while a
    print(a)
// or
while a => print(a)
// or
while(a) => print(a)
// or
while(a){
    print(a)
}
// or
while a {
    print(a)
}
```

#### Functions, `func` keyword
To define a function in Yocto you need to use `func` keyword:
```
func a //...
```
Arguments may be captured in parentheses or not. Examples are equal:
```
func a(p1, p2) {
    print(p1)
    print(p2)
}
// or
func a p1, p2 {
    print(p1)
    print(p2)
}
```
As `if` and `while`, function declaration may be one-line too, with the same rules:
```
func a(p1, p2) => print(p1, p2)
// or
func a p1, p2 => print(p1, p2)
```

It's also possible to declare function that does not receive arguments, like that:
```
func a{
    print('no args')
}
// or
func a => print('no args')
```

##### Default values
Default values may be written as any of the ways below:
```
func a(p1 = 'a', p2 = 'b'){
    print(p1)
    print(p2)
}
// or (white-spaces here do not matter)
func a p1 = 'a', p2 = 'b' // ...
```
Anyway, here's one common rule: **After parameter with default value all parameters MUST have default value**
In Ruby it's possible to set default value for first argument and not to set it for second one.
But, firstly, Yocto interpreter is not so smart, and secondly, I think it's not as much convenient as bad practice and may be confused.
So, most likely, in Yocto it will never be implemented.

#### Function calls
In Yocto function calls require parentheses(`()`), it's not possible to do it as in Ruby, even no-argument functions require parentheses.

### Types

#### Primitives
(primitives are objects too, everything is an object)

List of primitives:
- Null
- Bool
- Int
- Float (64-bit precision `double`)
- String

Why I didn't implement boolean as two separate classes like in Ruby (TrueClass, FalseClass)?
Mostly, because I want to have type hinting and it's more convenient to have a single class to use it as type. Anyway, this problem can be solved, I can create TrueClass/FalseClass and global type value for boolean to use in type hinting.

### Types conversion
For explicit type convertion every object has or can overload special convertion functions (they are same to Ruby's):
- `to_s` to string
- `to_i` to int
- `to_b` to boolean
- `to_f` to float

In Ruby there's the thing that I don't like at all. The Perl problem of multiple ways.
I really want to make Yocto "funny" to use as Ruby is, but I want to make it as strict as I can.

### Magic methods

#### Operator overloading
Operator overloading can be implemented with operator magic functions:
- `__add` - `+`
- `__sub` - `-`
- `__mul` - `*`
- `__div` - `/`
- `__mod` - `%`
- `__pow` - `**`

## Yocto code recomendations
Since Yocto provides different ways to do the same thing, I want to describe some rules about code writing for stringency.

### Single-line blocks
Examples with writing rules:
```
// 1. Classic. Use it whenever you want, even for one-line blocks.
if(1){
    print("true")
    print("true")
}else{
    print("false")
}

// 2. Flatness (Classic one-line).
if(1) print("true")
else print("false")

// 3. No parenthesis! Use this if you don't like parentheses.
// 3.1. One-line
if 1
    print("true")
else
    print("false")
// 3.2. Multi-line
if 1 {
    print("true")
    print("true")
}else{
    print("false")
    print("false")
}

// 4. Non-parentheses flat one-line. If you write code avoiding parenthesis, then use this syntax.
if 1 => print("true")
else print("false")

// 5! Don't write so. It's verbose.
if(1) => print("true")
else print("false")
// or
if(1) => {
    // ...
}
```

Briefly:
Don't use non-parenthesised syntax with parenthesised syntax together.
Just choose one of them and use it everywhere.
Use `=>` only for one-line blocks in non-parenthesised syntax.

All this rules are applicable to function declaration and while statement.

## Some important rules about code

### Parser
- NEVER skip newline after last operator in statement (statements )

## Notes

### How does class constructor works
The function `new` always exists and can be called as `ClassName()`.
But, the user-defined constructor calls after the default constructor.

## Personal reminders
- If there's Segmentation fault about unique_ptr, probably problem is in try to copy or multiple storing.
- When parser expect semi (`;` or `[new line]`) don't forget about checking for `!eof()`