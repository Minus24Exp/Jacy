# Tests
To create a test just make a yocto file inside a `test/[type of test]` directory, where `[type of test]` can be: `lexer`, `parser` or `eval`.
But `parser` includes `lexer` too and `eval` includes the whole process.

Anyway, `lexer` tests used rarely, mostly there're `parser` tests.