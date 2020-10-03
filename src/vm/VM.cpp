#include "vm/VM.h"

VM::VM() {
    // slots.emplace(slots.begin() + print_offset, Value{Type::ObjFunc, std::make_shared<Func>()});
}

void VM::push(const Value & val) {
    stack.push(val);
}

Value VM::top() {
    return stack.top();
}

Value VM::pop_stack() {
    Value value = top();
    stack.pop();
    return value;
}

//////////////////
// Instructions //
//////////////////
void VM::load_null() {
    push(NullConst);
}

void VM::load_bool(bool value) {
    push(value ? TrueConst : FalseConst);
}

void VM::load_int(int64_t value) {
    push(Value{Type::Int, value});
}

void VM::load_float(double value) {
    push(Value{Type::Float, value});
}

void VM::load_string(const char * value) {
    push(Value{Type::String, value});
}

void VM::load_var(uint64_t offset) {
    push(slots[slots.size() - offset - 1]);
}

void VM::store_var(uint64_t offset) {
    slots[slots.size() - offset - 1] = pop_stack();
}

void VM::pop() {
    pop_stack();
}

void VM::call(uint8_t args_count) {
    std::vector<Value> args;
    for (int i = 0; i < args_count; i++) {
        args.push_back(pop_stack());
    }

    Value func = pop_stack();
    if (func.type != Type::ObjFunc) {
        throw JacyException("Unable to use as a function");
    }

    push(std::dynamic_pointer_cast<Func>(func.obj())->call(args));
}