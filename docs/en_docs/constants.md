# Constants and Literals

Yocto's literals:
- null (singleton instance of NullClass)
- Bool (`true`/`false`)
- Int (64-bit precision signed number, `long`)
- Float (64-bit precision signed real number, `double`)
- String

All the literals reference to the same object if they have same value.

For example: `print('a' === 'a')` (`===` is reference equality)
Will print `true`, because literals with the same value reference to the same object.