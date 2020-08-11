# Types
Objects that can be used as type:
* user-defined classes
* built-in types (are also classes)
* `type` - type-alias variable declared with `type` keyword 

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
In Yocto it's possible to declare some kind of "alias" for type.
For example, I want to create a type that can be Int or Bool.
Here's the syntax I need to apply:
```
type intOrBool = Int | Bool
```