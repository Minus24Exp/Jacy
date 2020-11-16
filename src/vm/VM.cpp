#include "vm/VM.h"

VM::VM() : log("VM", options.log) {
    call_frames.push_back({});
    frame = call_frames.begin();
}

void VM::_nop() {}

void VM::_pop() {
    pop();
}

void VM::_null_const() {
    push(Null);
}

void VM::_false_const() {
    push(False);
}

void VM::_true_const() {
    push(True);
}

void VM::_int_const() {
    const auto & int_const = read_int_const();
    push(std::make_shared<Int>(int_const));
}

void VM::_float_const() {
    const auto & float_const = read_float_const();
    push(std::make_shared<Float>(float_const));
}

void VM::_string_const() {
    const auto & string_const = read_string_const();
    push(std::make_shared<String>(string_const));
}

void VM::_define_global() {
    const auto & global_name = read_string_const();
    globals[global_name->value] = nullptr;
}

void VM::_load_global() {
    const auto & global_name = read_string_const();
    try {
        push(globals.at(global_name->value));
    } catch (std::out_of_range & e) {
        error(global_name->value + " is not defined");
    }
}

void VM::_store_global() {
    const auto & global_name = read_string_const();
    try {
        globals.at(global_name->value) = top();
    } catch (std::out_of_range & e) {
        error(global_name->value + " is not defined");
    }
}

void VM::_load_local() {
    const auto & slot = read8();
    try {
        push(frame->slots.at(slot));
    } catch (std::out_of_range & e) {
        // TODO: Update when locals names will be done
        error("Unable to resolve local");
    }
}

void VM::_store_local() {
    const auto & slot = read8();
    try {
        frame->slots.at(slot) = top();
    } catch (std::out_of_range & e) {
        error("Unable to resolve local");
    }
}

void VM::_jump() {
    const auto & offset = read8();
    ip += offset;
}

void VM::_jump_false() {
    const auto & offset = read8();
    if (!top()->to_b()) {
        ip += offset;
    }
}

void VM::_invoke() {
    // TODO
}

void VM::_invoke_nf() {
    uint64_t arg_count = read8();
    std::shared_ptr<NativeFunc> func = std::static_pointer_cast<NativeFunc>(top(arg_count));
    std::vector<value_ptr> args = read_args(arg_count);
    value_ptr value = func->body(args);
    // TODO: !!! Create `void`, DO NOT USE NULL
    value = value ? value : Null;
    push(value);
}

void VM::_invoke_method() {
    // TODO
}

void VM::_get_property() {
    // TODO
}

void VM::_set_property() {
    // TODO
}

////////////
// Errors //
////////////
void VM::error(const std::string & msg) {
    // TODO: Current file
    // TODO: Position tracking
    throw RuntimeError(msg, {0, 0}, "<main>");
}
