# Types
Objects that can be used as type:
* user-defined classes
* built-in types (are also classes)
* type-alias - variable declared with `type` keyword 

Example:
```
Int - type
Bool - type

class A {}
A - type

val a = A()
a - not a type

type MyType = A | Bool
MyType - type
```

## Type declaration (aliases)
In Jacy it's possible to declare some kind of "alias" for type.
For example, I want to create a type that can be Int or Bool.
Here's the syntax I need to apply:
```
type intOrBool = Int | Bool
```

## Comparison (types and inheritance)
To compare two types (objects) just use `==` (equality) or `===` (reference equality) operators.
So, as types are also objects, comparison is same.

To check if object is instance of some type or inherit some type, use `is` operator.

Example:
```
class A {}
val a = A()
print(a is A) // true

val int_num = 1
val float_num = 1.0
print(int_num is Int) // true
print(float_num is Float) // true

type IntOrBool = Int | Bool
print(1 is IntOrBool) // true
print(true is IntOrBool) // true
```