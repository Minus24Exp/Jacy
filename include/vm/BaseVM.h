#ifndef BASEVM_H
#define BASEVM_H

#include "bytecode/constant.h"
#include "Exception.h"

#include <vector>
#include <map>

/**
 * BaseVM
 * Base class for Disasm and VM
 * It simplifies the byte-code reading by branching opcodes to functions,
 * by reading byte-code using overridden reader-functions.
 *
 * Some reader-functions need to be overridden 'cause of difference between Disasm and VM:
 * - Disasm at first just dumps all constants (including function constants and their inner byte-code) and prints the main byte-code
 * - VM goes to functions and create CallFrame thus it needs to read differently: from main byte-code or from function byte-code
 *
 * So, in spite of what was before, Disasm must not have anything in common with run-time, so no stack, call frames, etc.
 */

namespace jc::vm {
    class BaseVM {
    public:
        BaseVM();
        virtual ~BaseVM() = default;

        virtual void eval(const bytecode::Chunk & _chunk);

    protected:
        // Opcodes //
        virtual void before_eval() {}
        virtual void before_opcode(bytecode::OpCode opcode) {}
        virtual void after_opcode() {}
        virtual void unknown_opcode(uint8_t byte);
        virtual void _nop() = 0;
        virtual void _pop() = 0;
        virtual void _null_const() = 0;
        virtual void _false_const() = 0;
        virtual void _true_const() = 0;
        virtual void _int_const() = 0;
        virtual void _float_const() = 0;
        virtual void _string_const() = 0;
        virtual void _define_global() = 0;
        virtual void _load_global() = 0;
        virtual void _store_global() = 0;
        virtual void _get_upvalue() = 0;
        virtual void _set_upvalue() = 0;
        virtual void _close_upvalue() = 0;
        virtual void _closure() = 0;
        virtual void _load_local() = 0;
        virtual void _store_local() = 0;
        virtual void _jump() = 0;
        virtual void _jump_false() = 0;
        virtual void _invoke() = 0;
        virtual void _invoke_nf() = 0;
        virtual void _invoke_nf_method() = 0;
        virtual void _invoke_method() = 0;
        virtual void _get_property() = 0;
        virtual void _set_property() = 0;

        // Bytecode //
        bytecode::Chunk chunk;
        uint8_t peek();
        uint8_t read();
        uint16_t read2();
        uint32_t read4();
        uint64_t read8();

        virtual bytecode::bytelist_it peek_it() = 0;
        virtual void advance(int distance) = 0;

        // Constants //
        bytecode::constant_ptr read_const();
        std::shared_ptr<bytecode::IntConstant> read_int_const();
        std::shared_ptr<bytecode::FloatConstant> read_float_const();
        std::shared_ptr<bytecode::StringConstant> read_string_const();
        std::shared_ptr<bytecode::StringConstant> get_string_const(uint32_t offset);

        // Errors //
        static void error(const std::string & msg);
    };
}

#endif // BASEVM_H
