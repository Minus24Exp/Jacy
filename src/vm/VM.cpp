#include "vm/VM.h"

VM::VM() {
    call_frames.push_back({});
    frame = call_frames.begin();
}

void VM::eval(const Chunk & chunk) {
    this->chunk = chunk;

    while (ip < chunk.code.size()) {
        const auto & opcode = static_cast<OpCode>(read());
        switch (opcode) {
            case OpCode::NOP: break;
            case OpCode::Pop: {
                pop();
            } break;
            case OpCode::NullConst: {
                push(Null);
            } break;
            case OpCode::FalseConst: {
                push(False);
            } break;
            case OpCode::TrueConst: {
                push(True);
            } break;
            case OpCode::IntConst: {
                const auto & int_const = read_int_const();
                push(std::make_shared<Int>(int_const));
            } break;
            case OpCode::FloatConst: {
                const auto & float_const = read_float_const();
                push(std::make_shared<Float>(float_const));
            } break;
            case OpCode::StringConst: {
                const auto & string_const = read_string_const();
                push(std::make_shared<String>(string_const));
            } break;
            case OpCode::DefineGlobal: {
                const auto & global_name = read_string_const();
                globals[global_name->value] = nullptr;
            } break;
            case OpCode::LoadGlobal: {
                const auto & global_name = read_string_const();
                const auto & found = globals.find(global_name->value);
                if (found == globals.end() || !found->second) {
                    error(global_name->value + " is not defined");
                }
                push(found->second);
            } break;
            case OpCode::StoreGlobal: {
                const auto & global_name = read_string_const();
                const auto & found = globals.find(global_name->value);
                if (found == globals.end()) {
                    error(global_name->value + " is not defined");
                }
                globals.at(global_name->value) = top();
            } break;
            case OpCode::LoadLocal: {
                const auto & slot = read8();
                try {
                    push(frame->slots.at(slot));
                } catch (std::out_of_range & e) {
                    // TODO: Update when locals names will be done
                    error("Unable to resolve local");
                }
            } break;
            case OpCode::StoreLocal: {
                const auto & slot = read8();
                frame->slots.at(slot) = top();
            } break;
            case OpCode::Jump: {
                const auto & offset = read8();
                ip += offset;
            } break;
            case OpCode::JumpFalse: {
                const auto & offset = read8();
                if (!top()->to_b()) {
                    ip += offset;
                }
            } break;
            case OpCode::Invoke: {
                throw DevError("Invoke is not still implemented");
            } break;
            case OpCode::InvokeNF: {
                uint64_t arg_count = read8();
                std::shared_ptr<NativeFunc> func = std::static_pointer_cast<NativeFunc>(top(arg_count));
                std::vector<value_ptr> args = read_args(arg_count);
                value_ptr value = func->body(args);
                // TODO: !!! Create `void`, DO NOT USE NULL
                value = value ? value : Null;
                push(value);
            } break;
            case OpCode::GetProperty: {
                // TODO: GetProperty
            } break;
            case OpCode::SetProperty: {
                // TODO: SetProperty
            } break;
            default: break;
        }
    }
}

////////////
// Errors //
////////////
void VM::error(const std::string & msg) {
    // TODO: Current file
    // TODO: Position tracking
    throw RuntimeError(msg, {0, 0}, "<main>");
}
