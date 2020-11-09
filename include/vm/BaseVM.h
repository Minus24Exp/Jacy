#ifndef BASEVM_H
#define BASEVM_H

#include "vm/Value.h"
#include "compiler/opcode.h"
#include "Exception.h"
#include "vm/globals.h"
#include <vector>
#include <map>

struct CallFrame {
    std::vector<value_ptr> slots;
};

class BaseVM {
public:
    BaseVM() = default;
    virtual ~BaseVM() = default;

    virtual void eval(const Chunk & chunk) = 0;

protected:
    // Bytecode
    Chunk chunk;
    size_t ip{0};
    uint8_t peek() const;
    opcode_it peek_it();
    void advance(int distance = 1);
    uint8_t read();
    uint16_t read2();
    uint32_t read4();
    uint64_t read8();

    // Stack
    std::vector<value_ptr> stack;
    void push(const value_ptr & value);
    value_ptr pop();
    value_ptr top(uint64_t offset = 0);

    std::map<std::string, value_ptr> globals{jcGlobals};

    // Callframes
    std::vector<CallFrame> call_frames;
    std::vector<CallFrame>::iterator frame;

    // Constants
    constant_ptr read_const();
    std::shared_ptr<IntConstant> read_int_const();
    std::shared_ptr<FloatConstant> read_float_const();
    std::shared_ptr<StringConstant> read_string_const();

    // Errors
    static void error(const std::string & msg);
};

#endif // BASEVM_H
