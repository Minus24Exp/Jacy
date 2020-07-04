<p align="center">
	<img src="img/Yocto.png" width="250">	
</p>

# Yocto

10⁻²⁴ size programming language written in pure C++.

__Yocto implementation with AST-Walker.__

## Architecture
- Everything is an Object, primitives are objects too and all operators implemented as magic functions
- The compiler always inside a function. The top-level code is always in some kinda `main()` function

### Control flow

#### `if`/`elif`(`else if`)/`else`
In Yocto `if` is an expression.
It means that it can be used as:
```
var a = if() //...
```

`elif` is syntax sugar that automatically preprocessing to `else if` on lexing level.

`if`/`elif`/`else` can have one-line body:
```
if(a) print(a)
else print('nope')
```

`if`/`elif` can have conditions without parenthesis:
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
```

Important note: As `elif` converts to `else if` at lexing level then all errors about it will
reference to `else`. Maybe I'll solve it in the future.

### Types

#### Primitives
(primitives are objects too, everything is an object)

They're three primitives:
- string
- bool
- int
- float (64-bit precision)

### Types conversion
For explicit type convertion every object has or can overload special convertion functions (they are pretty same to Ruby's):
- `to_s` to string
- `to_i` to int
- `to_b` to boolean
- `to_f` to float

### Magic methods

#### Operator overloading
Operator overloading can be implemented with operator magic functions:
- `__add` - `+`
- `__sub` - `-`
- `__mul` - `*`
- `__div` - `/`
- `__mod` - `%`
- `__pow` - `**`

## Some important rules about code

### Parser
- NEVER skip newline after last operator in statement (statements )

## TODO
Move all literal object to one `Literal.h` ro reduce files count.

## Think about it
- General: If FuncDecl stored Token instead of Identifier then it will be possible to use not only ids for function names and e.g. operator overloading looked like: `func +()`
- Parser: Some stmts and exprs like `if` and `while` use the same syntax sugar as different ways to capture condition, maybe it's possible to move this syntax parsing to general function (maybe not...)
- Parser: What about functions `skip_if_op` and `skip_if_kw`?

## Personal reminders
- If there's Segmentation fault about unique_ptr, probably problem is in try to copy or multiple storing.
- When parser expect semi (`;` or `[new line]`) don't forget about checking for `!eof()`