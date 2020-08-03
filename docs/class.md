# Classes and instances

## Virtual-`this`
What is virtual-`this`?
Virtual-`this` is used to allow user avoid writing `this` before all class fields.
It's not possible to access virtual-`this` directly and it exists only inside instance method.

Example:
```
// Using `this`
class A {
    val my_name = "Jacy"

    func show {
        print(this.my_name)
    }
}

// Using virtual-this
class A {
    val my_name = "Jacy"

    func show {
        print(my_name)
    }
}
```

Virtual-`this` allows you not only to get instance fields, you can also assign them.
But the order of priority is "Name exists in function scope - use it, otherwise try to find it in virtual-`this`. Found? - use it"

Example:
```
class A {
    var name = "Unknown"

    func set_name(name) => this.name = name
    func show => print(name)
}
```
This is really useful, you only need to write `this` when there's variable with same name in scope.

It also perfectly works with inheritance.
Example:
```
class B {
    var name = "Unknown"
}

class A : B {
    func set_name(name) => this.name = name
    func show => print(name)
}
```