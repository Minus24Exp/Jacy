#include "vm/Value.h"

Value intValue(long i) {
    Value value;
    memcpy(&value, &i, sizeof(long));
    return value;
}

Value floatValue(double d) {
    Value value;
    memcpy(&value, &d, sizeof(double));
    return value;
}