#include "vm/VM.h"

VM::VM() {}

void VM::push(Value val) {
    stack.push(val);
}

void VM::const_null() {
    push({ValueType::Null});
}

void VM::const_bool(bool value) {
    push({ValueType::Bool, value});
}

void VM::const_int(yo_int value) {
    push({ValueType::Int, value});
}

void VM::const_float(double value) {
    push({ValueType::Float, value});
}

void VM::const_string(const std::string & value) {
    push({ValueType::String, value});
}

void VM::load(uint64_t offset) {
    
}

void VM::store(uint64_t offset) {
    
}
