#include "vm/Disasm.h"

Disasm::Disasm() : log("Disasm", options.log) {}

Disasm::Disasm(const DisasmOptions & options) : Disasm() {
    this->options = options;
}

void Disasm::before_eval() {
    std::cout << "=== Chunk ===" << std::endl;

    if (options.pure_dump) {
        std::cout << "-- Pure code --" << std::endl;
        int div = 0;
        for (const auto & byte : chunk.code) {
            std::cout << std::hex << static_cast<int>(byte) << " " << std::dec;
            div++;
            if (div == 4) {
                div = 0;
                std::cout << std::endl;
            }
        }
        if (div != 0) {
            std::cout << std::endl;
        }
    }

    std::cout << "-- Constant Pool --" << std::endl;
    uint64_t cp_offset = 0;
    for (const auto & constant : chunk.constants) {
        std::cout << cp_offset++ << " - " << static_cast<int>(constant->type) << " - " << constant->to_string() << std::endl;
    }

    std::cout << "-- Code --" << std::endl;
}

void Disasm::before_opcode(OpCode opcode) {
    try {
        std::cout << opcode_names.at(opcode) << " ";
    } catch (std::out_of_range & e) {
        throw DevError("Unknown opcode: " + std::to_string(static_cast<uint8_t>(opcode)));
    }
}

void Disasm::after_opcode() {
    std::cout << std::endl;
}

void Disasm::_nop() {}

void Disasm::_pop() {
    pop();
}

void Disasm::_null_const() {
    push(Null);
}

void Disasm::_false_const() {
    push(False);
}

void Disasm::_true_const() {
    push(True);
}

void Disasm::_int_const() {
    const auto & int_const = read_int_const();
    push(std::make_shared<Int>(int_const));
    std::cout << top()->to_string();
}

void Disasm::_float_const() {
    const auto & float_const = read_float_const();
    push(std::make_shared<Float>(float_const));
    std::cout << float_const->value;
}

void Disasm::_string_const() {
    const auto & string_const = read_string_const();
    push(std::make_shared<String>(string_const));
    std::cout << string_const->value;
}

void Disasm::_define_global() {
    const auto & global_name = read_string_const();
    std::cout << global_name->value;
    globals[global_name->value] = nullptr;
}

void Disasm::_load_global() {
    const auto & global_name = read_string_const();
    std::cout << global_name->value;
    try {
        push(globals.at(global_name->value));
        std::cout << " (" << globals.at(global_name->value)->to_string() + ")";
    } catch (std::out_of_range & e) {
        std::cout << " (UNDEFINED)";
    }
}

void Disasm::_store_global() {
    const auto & global_name = read_string_const();
    std::cout << global_name->value;
    try {
        globals.at(global_name->value) = top();
        std::cout << " = " << top()->to_string();
    } catch (std::out_of_range & e) {
        std::cout << " = (UNDEFINED)";
    }
}

void Disasm::_load_local() {
    const auto & slot = read8();
    std::cout << slot;
    // TODO: Use real values
    push(Null);
}

void Disasm::_store_local() {
    const auto & slot = read8();
    std::cout << slot;
    std::cout << " ";
    std::cout << top()->to_string();
}

void Disasm::_jump() {
    const auto & offset = read8();
    std::cout << offset;
}

void Disasm::_jump_false() {
    const auto & offset = read8();
    std::cout << offset << " (" << top()->to_string() << " - " << (top()->to_b() ? "true" : "false") << ")";
}

void Disasm::_invoke() {
    uint64_t arg_count = read8();
    value_ptr func = top(arg_count);
    std::cout << func->to_string() << "(";
    for (uint64_t i = 0; i < arg_count; i++) {
        std::cout << top(arg_count - i - 1)->to_string();
    }
    std::cout << ")";
}

void Disasm::_invoke_nf() {
    // TODO: ?
    _invoke();
}

void Disasm::_invoke_method() {
    // TODO: !?!
    _invoke();
}

void Disasm::_get_property() {
    std::cout << top()->to_string() << "." << read_string_const()->value;
}

void Disasm::_set_property() {
    std::cout << top(1)->to_string() << "." << read_string_const()->value << " = " << top()->to_string();
}
