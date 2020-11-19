# Operator overloading

To overload behavior of operator you can add an `operator` function with specified operator identifier as name.

Syntax example:
```
func operator+(other)

// You can put white-space before operator:
func operator -(other)
```
Also, for `operator` functions `func` keyword can be omitted:
```
operator+(other)
```

### `operator` functions as class methods
```
class A {
    val my_val = 1    

    func operator+(other_val: Int) => my_val + other_val
}

val a = A()
print(a + 1)
```
This code prints `2`.

### `operator` functions as raw functions [FUTURE]
The cool think that you don't have to create a method or extension-function to overload operator.
If some class is not extensible it would impossible to overload its operator.
So, you can define an `operator` function that receives lhs and rhs, and applies some code on them.
Compiler will automatically find function that corresponds to use case with an overloaded operator.

```
func operator+(a: MyClass, b: MyClass) => // ...

// Now you use it as:
val a = MyClass()
val b = MyClass()
print(a + b)
```
The result will be whatever you've written inside `operator+` body.

### Important information about `operator` functions:
- 1.a. `operator` functions must not change its object either increment and decrement operators.
- 1.b. `operator` functions must just return output value.
- 2.a. For all operator function parameters type must be specified, and it must not be `final` class.
- 2.b. You cannot overload basic operators, like integer addition, etc.

### Operators table
Operator | Name | Example | Overloadable? | Notes
:------: | :---: | :---: | :-----------: | :---
`+` | Addition (infix) | `a + b` | yes | 
`-` | Subtraction (infix) | `a - b` | yes |
`*` | Multiplication | `a * b` | yes |
`/` | Division | `a / b` | yes |
`%` | Modulo | `a % b` | yes |
`**` | Exponentiation | `a ** b` | yes |
`!` | Not | `!a` | yes |
`~` | Bitwise not | `~a` | yes
`+` | Unary plus | `+a` | yes
`-` | Unary minus | `-a` | yes
`++` | Increment | `a++` or `++a` | \*yes
`--` | Decrement | `a--` or `--a` | \*yes
`+=` | Plus assign | `a += b` | no | (`a = a + b` will be used if exists)
`-=` | Minus assign | `a -= b` | no | (`a = a - b` will be used if exists)
`*=` | Times assign | `a *= b` | no | (`a = a * b` will be used if exists)
`/=` | Division assign | `a /= b` | no | (`a = a / b` will be used if exists)
`%=` | Modulo assign | `a %= b` | no | (`a = a % b` will be used if exists)
`**=` | Power assign | `a **= b` | no | (`a = a ** b` will be used if exists)
`in` | In (includes) | `a in b` | yes
`!in` | Not in (doesn't include) | `a !in b` | no | (`!(a in b)` will be used if exists)
`[]` | Get element | `a[b]` or `a[foo, bar, ...]` | yes | There's multiple indexed access (`a[foo, bar, bax]`)
`[]=` | Set element | `a[b] = c` or `a[foo, bar, ...] = c` | yes | There's multiple indexed access (`a[foo, bar, bax] = b`)
`()` | Invoke | `a(...)` | yes | 
`==` | Equal | `a == b` | yes |
`!=` | Not equal | `a != b` | no | (`!(a == b)` will be used if exists)
`===` | Reference equal | `a === b` | no | 
`!==` | Not reference equal | `a !== b` | no |
`>` | Greater than | `a > b` | \*\*yes
`<` | Less than | `a < b` | \*\*yes
`>=` | Greater than or equal | `a >= b` | \*\*yes
`<=` | Less than or equal | `a <= b` | \*\*yes
`<=>` | Compare to (spaceship) | `a <=> b` | \*\*yes | If less return -1, if equals return 0, if greater return 1
`<<` | Left shift | `a << b` | yes |
`>>` | Right shift | `a >> b` | yes |
`&&` | And | `a && b` | no |
<code>&#124;&#124;</code> | Or | <code>a &#124;&#124; b</code> | no |
`&` | Bitwise and | `a & b` | yes |
`^` | Xor | `a ^ b` | yes |
<code>&#124;</code> | Bitwise or / type union | yes |
`as` | As (cast) | `a as b` | no |
`as?` | As nullable (cast) | `a as? b` | no
`is` | Type check | `a is b` | no
`!is` | Negated type check | `a !is b` | no
<code>&#124;></code> | Pipeline | <code>a &#124;> b</code> | no | Calls function `b` by passing `a` as argument
`.` | Dot notation | `a.b` | no |


\* Increment/decrement and augmented assignment `operator` function must not change object values, they must return a value that will be assigned to the variable on which the operation was used.
\*\* If spaceship (`<=>`) is overloaded, `>`, `<`, `>=` and `<=` overloading can be omitted.

##### Why some operators are not overloadable
- If operator has its simple part like `+` for `+=` it would be non-solid, if `+` and `+=` could be overloadable separately.
- `!in` is not overloadable by the same reason. We have `in`, we don't want to make possible to describe different behaviors for opposite operators.


### Complex cases of operator overloading

#### Different list accesses

##### Access by index

##### Access by range (slices)
