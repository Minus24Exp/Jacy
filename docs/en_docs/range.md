# Range

## Range operators

Available range operators:
- `..` - All-inclusive
- `>..` - Left-exclusive
- `..<` - Right-exclusive
- `>.<` - Both-sides-exclusive

All the operators are overloadable and have corresponding functions:
- `operator ..(rhs)`
- `operator >..(rhs)`
- `operator ..<(rhs)`
- `operator >.<(rhs)`

