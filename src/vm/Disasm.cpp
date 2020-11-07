#include "vm/Disasm.h"

Disasm::Disasm() = default;

void Disasm::eval(const Chunk & chunk) {
    this->chunk = chunk;

    const auto & opcode = static_cast<OpCode>(read());
    std::cout << opcode_names.at(opcode) << " ";
    while (index < chunk.code.size()) {
        switch (opcode) {
            case OpCode::NOP: break;
            case OpCode::Pop: {
                pop();
            } break;
            case OpCode::IntConst: {
                const auto & int_const = read_int_const();
                std::cout << int_const->value;
            } break;
            case OpCode::FloatConst: {
                const auto & float_const = read_float_const();
                std::cout << float_const->value;
            } break;
            case OpCode::StringConst: {
                const auto & string_const = read_string_const();
                std::cout << string_const->value;
            } break;
            case OpCode::DefineGlobal: {
                const auto & global_name = read_string_const();
                std::cout << global_name->value;
                globals[global_name->value] = top();
                pop();
            } break;
            case OpCode::LoadGlobal: {
                const auto & global_name = read_string_const();
                std::cout << global_name->value;
                const auto & found = globals.find(global_name->value);
                if (found == globals.end()) {
                    std::cout << " (UNDEFINED)";
                } else {
                    std::cout << found->second.to_string();
                }
            } break;
            case OpCode::StoreGlobal: {
                const auto & global_name = read_string_const();
                std::cout << global_name->value;
                const auto & found = globals.find(global_name->value);
                if (found == globals.end()) {
                    std::cout << " (UNDEFINED)";
                } else {
                    std::cout << " " << top().to_string();
                }
            } break;
            case OpCode::LoadLocal: {
                const auto & slot = read8();
                std::cout << slot;
                // TODO: Use real values
                push(Value{});
            } break;
            case OpCode::StoreLocal: {
                const auto & slot = read8();
                std::cout << slot;
                std::cout << " ";
                std::cout << top().to_string();
            } break;
            case OpCode::Jump: {
                const auto & offset = read8();
                std::cout << offset;
            } break;
            case OpCode::JumpFalse: {
                const auto & offset = read8();
                std::cout << offset;
                std::cout << top().to_string() << " - maybe false:)";
            } break;
            case OpCode::Call: {
                uint64_t arg_count = read8();
                Value func = top(arg_count);
                std::cout << func.to_string() << "(";
                for (uint64_t i = 0; i < arg_count; i++) {
                    std::cout << top(arg_count - i).to_string();
                }
                std::cout << ")";
            } break;
            case OpCode::GetProperty: {
                std::cout << top().to_string() << "." << read_string_const()->value;
            } break;
            case OpCode::SetProperty: {
                std::cout << top(1).to_string() << "." << read_string_const()->value << " = " << top().to_string();
            } break;
            default: break;
        }
        std::cout << std::endl;
    }
}
