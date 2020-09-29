#include "vm/VM.h"

VM::VM() {}

void VM::push(Value val) {
    stack.push(val);
}

Value VM::top() {
    return stack.top();
}

void VM::load_const(uint8_t offset) {
    push(consts[offset]);
}

void VM::load_var(uint64_t offset) {
    push(slots[offset]);
}

void VM::store_var(uint64_t offset) {
    slots[offset] = top();
}
