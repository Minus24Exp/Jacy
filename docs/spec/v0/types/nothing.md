## Bottom type - `nothing`
`nothing` is the type that never exists. Do not confuse it with `null`, `nothing` is more like `void`.

`nothing` can be used to describe function that always throws an exception or runs infinite loop.
Anyway variables can hold `nothing` and you can pass `nothing` as parameter, but there's no reason to do that.

### Inference
`nothing` will be inferred for function as return type if it always throws an exception.
Example:
```
func myError(msg: string) {
    throw MyError(msg)
}
// Compiler infer it as
func myError(msg: string): nothing {
    throw MyError(msg)
}
```

| [< **Types**](index.md) | [`unit` >](unit.md) |
|:---:|:---:|
