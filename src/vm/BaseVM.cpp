#include "vm/BaseVM.h"

namespace jc::vm {
    BaseVM::BaseVM() = default;

    void BaseVM::eval(const bytecode::Chunk & _chunk) {
        this->chunk = _chunk;

        before_eval();

        while (true) {
            const auto & byte = read();
            const auto & opcode = static_cast<bytecode::OpCode>(byte);
            before_opcode(opcode);
            switch (opcode) {
                case bytecode::OpCode::NOP: {
                    _nop();
                } break;
                case bytecode::OpCode::Halt: {
                    // Note: Halt opcode does not have companion function
                    after_opcode();
                    return;
                } break;
                case bytecode::OpCode::Pop: {
                    _pop();
                } break;
                case bytecode::OpCode::NullConst: {
                    _null_const();
                } break;
                case bytecode::OpCode::FalseConst: {
                    _false_const();
                } break;
                case bytecode::OpCode::TrueConst: {
                    _true_const();
                } break;
                case bytecode::OpCode::IntConst: {
                    _int_const();
                } break;
                case bytecode::OpCode::FloatConst: {
                    _float_const();
                } break;
                case bytecode::OpCode::StringConst: {
                    _string_const();
                } break;
                case bytecode::OpCode::DefineGlobal: {
                    _define_global();
                } break;
                case bytecode::OpCode::LoadGlobal: {
                    _load_global();
                } break;
                case bytecode::OpCode::StoreGlobal: {
                    _store_global();
                } break;
                case bytecode::OpCode::LoadLocal: {
                    _load_local();
                } break;
                case bytecode::OpCode::StoreLocal: {
                    _store_local();
                } break;
                case bytecode::OpCode::LoadUpvalue: {
                    _get_upvalue();
                } break;
                case bytecode::OpCode::StoreUpvalue: {
                    _set_upvalue();
                } break;
                case bytecode::OpCode::CloseUpvalue: {
                    _close_upvalue();
                } break;
                case bytecode::OpCode::Closure: {
                    _closure();
                } break;
                case bytecode::OpCode::Jump: {
                    _jump();
                } break;
                case bytecode::OpCode::JumpFalse: {
                    _jump_false();
                } break;
                case bytecode::OpCode::Invoke: {
                    _invoke();
                } break;
                case bytecode::OpCode::InvokeNF: {
                    _invoke_nf();
                } break;
                case bytecode::OpCode::InvokeMethod: {
                    _invoke_method();
                } break;
                case bytecode::OpCode::GetProperty: {
                    _get_property();
                } break;
                case bytecode::OpCode::SetProperty: {
                    _set_property();
                } break;
                default: {
                    unknown_opcode(byte);
                }
            }
            after_opcode();
        }
    }

    void BaseVM::unknown_opcode(uint8_t byte) {
        throw DevError("Unknown opcode with byte: " + std::to_string(byte));
    }

    //////////////
    // Bytecode //
    //////////////
    uint8_t BaseVM::peek() {
        return *peek_it();
    }

    uint8_t BaseVM::read() {
        uint8_t b = peek();
        advance(1);
        return b;
    }

    uint16_t BaseVM::read2() {
        uint16_t s;
        std::copy(peek_it(), peek_it() + 2, reinterpret_cast<uint8_t*>(&s));
        advance(2);
        return s;
    }

    uint32_t BaseVM::read4() {
        uint32_t i;
        std::copy(peek_it(), peek_it() + 4, reinterpret_cast<uint8_t*>(&i));
        advance(4);
        return i;
    }

    uint64_t BaseVM::read8() {
        uint64_t l;
        std::copy(peek_it(), peek_it() + 8, reinterpret_cast<uint8_t*>(&l));
        advance(8);
        return l;
    }

    ///////////////
    // Constants //
    ///////////////
    /**
     * Read 4-byte constant offset and return constant
     * @return
     */
    bytecode::constant_ptr BaseVM::read_const() {
        // Note: read_const automatically reads constant offset and advances
        const auto & offset = read4();
        if (offset >= chunk.constant_pool.size()) {
            // TODO: Remove cout
            std::cout << std::endl << "offset: " << offset << ". size: " << chunk.constant_pool.size() << std::endl;
            throw DevError("Constant offset is out of constant pool bounds");
        }
        return chunk.constant_pool.at(offset);
    }

    std::shared_ptr<bytecode::IntConstant> BaseVM::read_int_const() {
        return std::static_pointer_cast<bytecode::IntConstant>(read_const());
    }

    std::shared_ptr<bytecode::FloatConstant> BaseVM::read_float_const() {
        return std::static_pointer_cast<bytecode::FloatConstant>(read_const());
    }

    std::shared_ptr<bytecode::StringConstant> BaseVM::read_string_const() {
        return std::static_pointer_cast<bytecode::StringConstant>(read_const());
    }

    std::shared_ptr<bytecode::StringConstant> BaseVM::get_string_const(uint32_t offset) {
        // TODO: Unite read_const and get_const
        if (offset >= chunk.constant_pool.size()) {
            throw DevError("Constant offset is out of constant pool bounds");
        }
        return std::static_pointer_cast<bytecode::StringConstant>(chunk.constant_pool.at(offset));
    }

    std::shared_ptr<bytecode::FuncConstant> BaseVM::read_func_constant() {
        return std::static_pointer_cast<bytecode::FuncConstant>(read_const());
    }

    ////////////
    // Errors //
    ////////////
    void BaseVM::error(const std::string & msg) {
        // TODO: Use VM exceptions
        throw JacyException(msg);
    }
}
