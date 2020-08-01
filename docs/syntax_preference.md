# Syntax preference
Yocto's syntax is various. Sometimes you need to choose one way to write something, sometimes you should always prefer one way.

### Parentheses
Control flow expressions and function delcaration can be written with/without parentheses.
Example:
```
// C-like
if(true){
	print('yoyo')
}
// No-parentheses
if true {
	print('yoyo')
}
```
In the case of (not) using parentheses, you should choose one way and write all the code in the same style.

#### `()` or not ?
No words, just look at examples and choose what you like more:
```
// Standard `if`
if(true){
	print('yoyoyo')
}

// No-paren `if`
if true {
	print('yoyoyo')
}

// One-line standard `if`
if(true) print('yoyoyo')

// One-line no-paren `if`
if true => print('yoyoyo')

// Possible to write, but nonsensical
if(true) => print('yoyoyo')
```
So, as you can see, one-line no-paren `if` requires `=>`, because parser need to know when condition ends.

As far as not only if can be written without parentheses, look at these examples:
```
// While statement
while(true){
	print('yoyo, infinity')
}
while true {
	print('aaaaa')
}
while(true) print('oaoaoaoao')
while true => print('10')

// Function declaration
func a(){
	print('just a raw function')
}
func a {
	print('No parameters, so why do I need parentheses???')
}
func a(p1){
	print('Raw function with parameter')
}
func a p1 {
	print('No parentheses, but parameter is here...')
}
func a p1='default value' {
	print('Yes, it is possible')
}
func a => print('oaoaoa, meow')
func a p1 => print(p1 + ' is parameter')
func a p1='flip flops' => print('I will miss american tiktokers')
```

### Braces (Curly-brackets)
Not only parentheses are avoidable. So... Just examples:
```
// No-paren one-liner without braces
if true
	print('yeps')
while true
	print('minus ram')
// But, it's not possible to use this syntax with function declaration
```
