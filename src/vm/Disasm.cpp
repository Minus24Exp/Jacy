#include "vm/Disasm.h"

namespace jc::vm {
    Disasm::Disasm() : log("Disasm", options.log) {}

    Disasm::Disasm(const common::DisasmOptions & options) : Disasm() {
        this->options = options;
    }

    void Disasm::before_eval() {
        std::cout << "=== Chunk ===" << std::endl;

        if (options.pure_dump) {
            std::cout << "-- Pure code --" << std::endl;
            print_bytes(chunk.code);
        }

        std::cout << "-- Constant Pool --" << std::endl;
        uint64_t cp_offset = 0;
        for (const auto & constant : chunk.constant_pool) {
            if (constant->tag == bytecode::ConstTag::Func) {
                const auto & func = std::static_pointer_cast<bytecode::FuncConstant>(constant);
                std::cout << cp_offset++ << " - " << "<func:" << get_string_const(func->name_offset)->value + ">";
                print_bytes(func->code);
            } else {
                std::cout << cp_offset++ << " - " << constant->to_string() << std::endl;
            }
        }

        std::cout << "-- Code --" << std::endl;
    }

    void Disasm::before_opcode(bytecode::OpCode opcode) {
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

    void Disasm::_pop() {}

    void Disasm::_null_const() {}

    void Disasm::_false_const() {}

    void Disasm::_true_const() {}

    void Disasm::_int_const() {
        const auto & int_const = read_int_const();
        std::cout << int_const->value;
    }

    void Disasm::_float_const() {
        const auto & float_const = read_float_const();
        std::cout << float_const->value;
    }

    void Disasm::_string_const() {
        const auto & string_const = read_string_const();
        std::cout << string_const->value;
    }

    void Disasm::_define_global() {
        const auto & global_name = read_string_const();
        std::cout << global_name->value;
    }

    void Disasm::_load_global() {
        const auto & global_name = read_string_const();
        std::cout << global_name->value;
    }

    void Disasm::_store_global() {
        const auto & global_name = read_string_const();
        std::cout << global_name->value;
    }

    void Disasm::_load_local() {
        const auto & slot = read8();
        std::cout << slot;
    }

    void Disasm::_store_local() {
        uint32_t slot = read4();
        std::cout << slot;
    }

    void Disasm::_get_upvalue() {
        uint32_t slot = read4();
        std::cout << slot;
    }

    void Disasm::_set_upvalue() {
        uint32_t slot = read4();
        std::cout << slot;
    }

    void Disasm::_close_upvalue() {

    }

    void Disasm::_closure() {

    }

    void Disasm::_jump() {
        const auto & offset = read8();
        std::cout << offset;
    }

    void Disasm::_jump_false() {
        const auto & offset = read8();
        std::cout << offset;
    }

    void Disasm::_invoke() {
        // TODO: Should I push result of invoke???
        uint32_t arg_count = read4();
        std::cout << arg_count;
    }

    void Disasm::_invoke_nf() {
        _invoke();
    }

    void Disasm::_invoke_method() {
        uint32_t arg_count = read4();
        const auto & method_name = read_string_const();

    }

    void Disasm::_invoke_nf_method() {
        _invoke_method();
    }

    void Disasm::_get_property() {
        std::cout << "[top]." << read_string_const()->value;
    }

    void Disasm::_set_property() {
        std::cout << "[top - 1]." << read_string_const()->value << " = " << "[top]";
    }

    void Disasm::print_bytes(const bytecode::byte_list & bytes) {
        int div = 0;
        for (const auto & byte : bytes) {
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

    // Byte-code reading //
    bytecode::bytelist_it Disasm::peek_it() {
        return chunk.code.begin() + ip;
    }

    void Disasm::advance(int distance) {
        ip += distance;
    }
}
