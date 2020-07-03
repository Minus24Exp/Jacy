<p align="center">
	<img src="img/Yocto.png" width="250">	
</p>

# Yocto

10⁻²⁴ size programming language written in pure C++.

__Yocto implementation with AST-Walker.__

## Architecture
- Everything is an Object, primitives are objects too and all operators implemented as magic functions
- The compiler always inside a function. The top-level code is always in some kinda `main()` function

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


## Think about it
If FuncDecl stored Token instead of Identifier then it will be possible to use not only ids for function names and e.g. operator overloading looked like: `func +()`.