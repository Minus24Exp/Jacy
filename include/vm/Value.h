#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <cstring>

const uint64_t SIGN_BIT = 0x8000000000000000;
const uint64_t QNAN = 0x7ffc000000000000;

using Value = uint64_t;

// Constants
const Value NullConst = QNAN | 1;
const Value FalseConst = QNAN | 2;
const Value TrueConst = QNAN | 3;

// Value conversion
Value intValue(long i);
Value floatValue(double d);

// Checkers
bool isInt();

#endif

94088928169176