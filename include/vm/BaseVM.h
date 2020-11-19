#ifndef BASEVM_H
#define BASEVM_H

#include "vm/Object.h"
#include "compiler/opcode.h"
#include "Exception.h"
#include "../../lib/globals.h"
#include <vector>
#include <map>

struct CallFrame {
    std::vector<object_ptr> slots;
};

class BaseVM {
public:
    BaseVM();
    virtual ~BaseVM() = default;

    virtual void eval(const Chunk & chunk);

protected:
    // Opcodes //
    virtual void before_eval() {}
    virtual void before_opcode(OpCode opcode) {}
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
    virtual void _load_local() = 0;
    virtual void _store_local() = 0;
    virtual void _jump() = 0;
    virtual void _jump_false() = 0;
    virtual void _invoke() = 0;
    virtual void _invoke_nf() = 0;
    virtual void _invoke_method() = 0;
    virtual void _get_property() = 0;
    virtual void _set_property() = 0;

    // Bytecode //
    Chunk chunk;
    size_t ip{0};
    uint8_t peek() const;
    opcode_it peek_it();
    void advance(int distance = 1);
    uint8_t read();
    uint16_t read2();
    uint32_t read4();
    uint64_t read8();

    // Stack //
    std::vector<object_ptr> stack;
    void push(const object_ptr & value);
    object_ptr pop();
    object_ptr top(uint64_t offset = 0);

    std::map<std::string, object_ptr> globals;

    // Call-frames //
    std::vector<CallFrame> call_frames;
    std::vector<CallFrame>::iterator frame;
    std::vector<object_ptr> read_args(uint64_t arg_count);

    // Constants //
    constant_ptr read_const();
    std::shared_ptr<IntConstant> read_int_const();
    std::shared_ptr<FloatConstant> read_float_const();
    std::shared_ptr<StringConstant> read_string_const();

    // Errors //
    static void error(const std::string & msg);
};

#endif // BASEVM_H
