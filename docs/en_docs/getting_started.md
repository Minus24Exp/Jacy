# Yocto Specification

## Grammar notations
All grammar notations are/must be written in ANTLR-like(not real antlr) syntax, because it is easier to read rather than BNF.
Anyway, here's some basics about it:
```
Regex-like operators:
() - grouping
* - any count of appearance (appears after syntax unit)
+ - one or more
? - optional (one or zero)
| - or operator
{n,m} - more than n (inclusive) and less than m (inclusive). m can be omitted.

Syntax units:
name: *syntax*
```

Note: Syntax does not require Lexer-grammar, so everything that is Token written as string. e.g. "import" is 'import', not IMPORT_TOKEN or something.

Syntax notations are nothing but just hints, you should still focus on documentation (all the syntax is described and explained in text)

Basic syntax-units:
```
# This is a comment

dec_digit: [0-9]
hex_digit: [0-9a-fA-F]

dec_digits: dec_digit (dec_digit | '_')* dec_digit

# Integer cannot start with 0
int: [1-9]+ (dec_digit | '_')* dec_digit

exponent_suffix: [eE] [+-]? dec_digits
float: dec_digit* '.' dec_digit+ exponent_suffix
     | dec_digit exponent_suffix

identifier: '_'{2,} [a-zA-Z0-9_]*
          | [a-zA-Z]+ [a-zA-Z0-9_]*

expression: 'any valid expression' (not described)
```