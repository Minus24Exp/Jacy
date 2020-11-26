## Int type - `int`
Int type represents integral numbers.

Examples:
```
1
-123

// Hexademical numbers
0xAF
-0x8C

// Binary numbers
0b1010
-0b01111

// Octal numbers
0o1231
-0o0772
```

#### Sized ints
**Sized integers are not available in version 0.0.1 and will be implemented in the future, now only `int` is available**

Several sized integer are available:
- `i8` - signed 8-bit integer
- `i16` - signed 16-bit integer
- `i32`/`int` - signed 32-bit integer
- `i64` - signed 64-bit integer
- `i128` - signed 128-bit integer
- `u8` - unsigned 8-bit integer
- `u16` - unsigned 16-bit integer
- `u32` - unsigned 32-bit integer
- `u64` - unsigned 64-bit integer
- `u128` - unsigned 128-bit integer

*Note*: Likely 128-bit sized integers won't be implemented when other sized integers will be implemented.


| [< `bool`](./bool.md) | [`float` >](./float.md) |
|:---:|:---:|
