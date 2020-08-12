# Destructuring

> __This feature is not implemented__

Destructuring is very useful feature.

## List destructuring
List destruction can be used with any object that indexed by integer.

Example:
```
// a = 1, b = 2, c = 3
var [a, b, c] = [1, 2, 3]
```

Note: Destructuring does not with assignment, but only with variable declaration.
> This won't compile
```
var a
[a] = [1]
```

## Dict destructuring
List destruction can be used with any object that indexed by string.

Example:
```
// a = 1, b = 2, c = 3
var {a, b, c} = {'a': 1, 'b': 2, 'c':, 3}

// a = 'str', b = null, c = false
var {a, b, c} = {'a': 'str', 'b': null, 'c': false}
```

Note: Destructuring does not with assignment, but only with variable declaration.
> This won't compile
```
var a
{a} = {'a': 'str'}
```