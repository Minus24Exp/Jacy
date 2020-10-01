#include "vm/VM.h"

VM::VM() {}

void VM::push(Value val) {
    stack.push(val);
}

Value VM::top() {
    return stack.top();
}

void VM::load_null() {
    push(NullConst);
}

void VM::load_bool(bool value) {
    push(value ? TrueConst : FalseConst);
}

void VM::load_int(int64_t value) {
}

void VM::load_float(double value) {
    // push(NullConst);
}

void VM::load_string(const char * value) {
    // push(NullConst);
}

void VM::load_var(uint64_t offset) {
    push(slots[offset]);
}

void VM::store_var(uint64_t offset) {
    slots[offset] = top();
}
