## Unit type - `unit`
`unit` is the type that has only one value.

`unit` can be used to describe function that returns no value. Despite `void`, `unit` is not "no value" type, it's a value but only one value for ever.

### Inference
- `unit` will be inferred as function return type if it returns no value.

Example:
```
// unit as return type inference
func foo() {
    return
}
// Compiler infer it as
func foo(): unit {
    return
}
```

| [< `nothing`](nothing.md) | [`null` >](null.md) |
|:---:|:---:|
