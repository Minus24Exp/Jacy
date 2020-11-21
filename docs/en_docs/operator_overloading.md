# Operator overloading

To overload behavior of operator you can add an `operator` function with specified operator identifier as name.

Syntax example:
```
func operator add(other)
```
Also, for `operator` functions `func` keyword can be omitted:
```
operator add(other)
```

### `operator` functions as class methods
```
class A {
    val my_val = 1    

    func operator add(other_val: Int) => my_val + other_val
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
func operator add(a: MyClass, b: MyClass) => // ...

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
Operator |  Name  | Identifier | Example | Overloadable? | Notes
:------: | :----: | :--------: | :-----: | :-----------: | :---
`+` | Addition (infix) | `add` | `a + b` | yes | 
`-` | Subtraction (infix) | `minus` | `a - b` | yes |
`*` | Multiplication | `times` | `a * b` | yes |
`/` | Division | `dic` | `a / b` | yes |
`%` | Modulo | `mod` | `a % b` | yes |
`**` | Exponentiation | `pow` | `a ** b` | yes |
`!` | Not | `not` | `!a` | yes |
`~` | Bitwise not | `inv` | `~a` | yes
`+` | Unary plus | `unaryPlus` | `+a` | yes
`-` | Unary minus | `unaryMinus` | `-a` | yes
`++` | Increment | `inc` | `a++` or `++a` | \*yes
`--` | Decrement | `dec` | `a--` or `--a` | \*yes
`+=` | Plus assign | --- | `a += b` | no | (`a = a + b` will be used if exists)
`-=` | Minus assign | --- | `a -= b` | no | (`a = a - b` will be used if exists)
`*=` | Times assign | --- | `a *= b` | no | (`a = a * b` will be used if exists)
`/=` | Division assign | --- | `a /= b` | no | (`a = a / b` will be used if exists)
`%=` | Modulo assign | --- | `a %= b` | no | (`a = a % b` will be used if exists)
`**=` | Power assign | --- | `a **= b` | no | (`a = a ** b` will be used if exists)
`in` | In (includes) | `in` | `a in b` | yes
`!in` | Not in (doesn't include) | --- | `a !in b` | no | (`!(a in b)` will be used if exists)
`[]` | Get element | `get` | `a[b]` or `a[foo, bar, ...]` | yes | There's multiple indexed access (`a[foo, bar, bax]`)
`[]=` | Set element | `set` | `a[b] = c` or `a[foo, bar, ...] = c` | yes | There's multiple indexed access (`a[foo, bar, bax] = b`)
`()` | Invoke | `invoke` | `a(...)` | yes | 
`==` | Equal | `equals` | `a == b` | yes |
`!=` | Not equal | --- | `a != b` | no | (`!(a == b)` will be used if exists)
`===` | Reference equal | --- | `a === b` | no | 
`!==` | Not reference equal | --- | `a !== b` | no |
`>` | Greater than | --- | `a > b` | \*\*not really
`<` | Less than | --- | `a < b` | \*\*not really
`>=` | Greater than or equal | --- | `a >= b` | \*\*not really
`<=` | Less than or equal | --- | `a <= b` | \*\*not really
`<=>` | Compare to (spaceship) | `compareTo` | `a <=> b` | \*\*yes | If less return -1, if equals return 0, if greater return 1
`<<` | Left shift | `shl` | `a << b` | yes |
`>>` | Right shift | `shr` | `a >> b` | yes |
`&&` | And | --- | `a && b` | no |
<code>&#124;&#124;</code> | Or | --- | <code>a &#124;&#124; b</code> | no |
`&` | Bitwise and | `bitAnd` | `a & b` | yes |
`^` | Xor | `xor` | `a ^ b` | yes |
<code>&#124;</code> | Bitwise or / type union | `bitOr` | yes |
`as` | As (cast) | --- | `a as b` | no |
`as?` | As nullable (cast) | --- | `a as? b` | no
`is` | Type check | --- | `a is b` | no
`!is` | Negated type check | --- | `a !is b` | no
<code>&#124;></code> | Pipeline | --- | <code>a &#124;> b</code> | no | Calls function `b` by passing `a` as argument
`.` | Dot notation | --- | `a.b` | no |


\* Increment/decrement and augmented assignment `operator` function must not change object values, they must return a value that will be assigned to the variable on which the operation was used.
\*\* For comparison operators like `>`, `<`, `>=` and `<=` you need to overload compareTo opeator (`<=>`) which covers their behavior.

#### Overloadable operators signatures


##### Why some operators are not overloadable
- If operator has its simple part like `+` for `+=` it would be non-solid, if `+` and `+=` could be overloadable separately.
- `!in` is not overloadable by the same reason. We have `in`, we don't want to make possible to describe different behaviors for opposite operators.


### Complex cases of operator overloading

#### Different list accesses

##### Access by index

##### Access by range (slices)
