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
// or
if a => print(a)
elif b => print(b)
else print('nope')
// or
if a => { print(a) }
elif b => { print(b) }
else { print('nope') }
```

So, all control flow statements can have one-line block if:
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
In Yocto function can be declared with `func` keyword:
```
func a //...
```
Arguments can be captured in parenthesis or not. Examples are equal:
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
As `if` and `while`, function declaration can be one-line too, with the same rules:
```
func a(p1, p2) => print(p1, p2)
// or
func a p1, p2 => print(p1, p2)
```

It's also possible to declare function that does not receive arguments this way:
```
func a{
	print('no args')
}
// or
func a => print('no args')
```

##### Default values
Default values can be written with any of ways below:
```
func a(p1 = 'a', p2 = 'b'){
	print(p1)
	print(p2)
}
// or
func a p1 = 'a', p2 = 'b' // ...
```
Anyway, here's one common rule: **After parameter with default value all parameters MUST have default value**
In Ruby it's possible to set default value for first argument and not to set it for second one.
But, firstly, Yocto interpreter is not so smart, and secondly, I think it's not as much convenient as bad practice and may be confused.
So, most likely, in Yocto it will never be implemented.

#### Function calls
In Yocto function calls require parenthesis(`()`), it's not possible to do it as in Ruby, even no-argument functions require parenthesis.

### Types

#### Primitives
(primitives are objects too, everything is an object)

They're three primitives:
- Null
- String
- Bool
- Int
- Float (64-bit precision `double`)

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

## Notes

### How does class constructor works
The function `new` always exists and can be called as `ClassName()`.
But, the user-defined constructor calls after the default constructor.

## TODO
- After first version "release" do general parsing tests and other tests too
- Add multi-threading (not in Yocto), to lex and parse at the same time as Interpreter register globals.
- Add namespaces
- Class extensions
- Think about and make if possible: "Change visitors pass by pointer to pass by reference to avoid segfault with nullptr"

## Think about it
- General: If FuncDecl stored Token instead of Identifier then it will be possible to use not only ids for function names and e.g. operator overloading looked like: `func +()`
- Parser: Some stmts and exprs like `if` and `while` use the same syntax sugar as different ways to capture condition, maybe it's possible to move this syntax parsing to general function (maybe not...)
- Parser: What about functions `skip_if_op` and `skip_if_kw`?
- General: If one day I'll add infix functions, then I can remove operators at all, and make all punctuation operator to punctuation tokens.

## Personal reminders
- If there's Segmentation fault about unique_ptr, probably problem is in try to copy or multiple storing.
- When parser expect semi (`;` or `[new line]`) don't forget about checking for `!eof()`