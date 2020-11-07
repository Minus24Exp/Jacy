#ifndef BASEVM_H
#define BASEVM_H

#include "vm/Value.h"
#include "compiler/opcode.h"
#include "Exception.h"
#include <vector>
#include <map>

class BaseVM {
public:
    BaseVM() = default;
    virtual ~BaseVM() = default;

    virtual void eval(const Chunk & chunk) = 0;

protected:
    // Bytecode
    Chunk chunk;
    size_t index{0};
    uint8_t peek() const;
    opcode_it peek_it();
    void advance(int distance = 1);
    uint8_t read();
    uint16_t read2();
    uint32_t read4();
    uint64_t read8();

    // Stack
    std::vector<Value> stack;
    void push(const Value & value);
    Value pop();
    Value top(uint64_t offset = 1);

    // Storages
    std::map<std::string, Value> globals;

    // Constants
    constant_ptr read_const();
    std::shared_ptr<IntConstant> read_int_const();
    std::shared_ptr<FloatConstant> read_float_const();
    std::shared_ptr<StringConstant> read_string_const();

    // Errors
    static void error(const std::string & msg);
};

#endif // BASEVM_H
