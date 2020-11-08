#ifndef OPCODE_H
#define OPCODE_H

#include <utility>
#include <vector>
#include <cstdint>
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

const int jump_space = 8;
using ByteList = std::vector<uint8_t>;
using opcode_it = ByteList::iterator;

struct Constant;
using constant_ptr = std::shared_ptr<Constant>;

/**
 * Bytecode spec
 * This bytecode is aimed at:
 * 1. Being evaluated as opcode object
 * 2. Being easy-writable/readable to/from file
 *
 * Bytecode file/list structure
 * -- (Here must be header info like version number, magic, etc.) --
 * constant_list_size
 * constant_list
 * code_size
 * code
 * attributes_count
 * attributes (names, line info, debug info)
 *
 * OPCODEs table
 * - CP - Constant Pool
 * - ST - Stack Top (ST = Stack[Stack.size - 1]. Can receive offset - ST[O] is Stack[Stack.size - O - 1])
 * - S - String constant offset
 * - L - Long (8-byte)
 *
 * -- Opcode --     | -- operands --
 * NOP              | [Nothing]
 * NullConst        | ST = NullConst
 * FalseConst       | ST = FalseConst
 * TrueConst        | ST = TrueConst
 * IntConst         | L -> ST = CP[L]
 * FloatConst       | L -> ST = CP[L]
 * StringConst      | L -> ST = CP[L]
 *
 * DefineGlobal     | S -> CP[S]
 * LoadGlobal       | S -> ST = globals[CP[S]]
 * StoreGlobal      | S -> globals[CP[S]] = ST
 * LoadLocal        | L -> ST = Frame.slots[L]
 * StoreLocal       | L -> Frame.slots[L] = ST
 *
 * Jump             | L -> IP += L
 * JumpFalse        | L -> ST is falsy ? IP += L
 *
 * Invoke           | L -> ST = Stack[L - 1].call(Stack[L..0])
 * InvokeNF         | L -> ST = NativeFunction(Stack[L - 1]).call(Stack[L..0])
 *
 * GetProperty      | S -> ST = ST.getProperty(CP[S])
 * SetProperty      | S -> ST[1].setProperty(CP[A], ST)
 */

enum class OpCode : uint8_t {
    NOP,
    Pop,

    NullConst,
    FalseConst,
    TrueConst,
    IntConst,
    FloatConst,
    StringConst,

    DefineGlobal,
    LoadGlobal,
    StoreGlobal,
    LoadLocal,
    StoreLocal,

    Jump,
    JumpFalse,

    Invoke,
    InvokeNF,

    GetProperty,
    SetProperty,
};

enum class ConstantType : uint8_t {
    Int,
    Float,
    String,
};

struct Constant {
    ConstantType type;
    explicit Constant(ConstantType type) : type(type) {}

    uint8_t type_code() const {
        return static_cast<uint8_t>(type);
    }
    virtual ByteList codegen() = 0;

    // Debug
    virtual std::string to_string() = 0;
};

struct IntConstant : Constant {
    long long value;

    explicit IntConstant(long long value) : Constant(ConstantType::Int), value(value) {}

    ByteList codegen() override {
        ByteList bytes = {type_code()};
        for (int i = 0; i < sizeof(value); i++) {
            bytes.push_back((value >> (i * 8u)) & 0XFFu);
        }
        return bytes;
    }

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct FloatConstant : Constant {
    double value;

    explicit FloatConstant(double value) : Constant(ConstantType::Float), value(value) {}

    ByteList codegen() override {
        ByteList bytes = {type_code()};
        const auto * byte_arr = reinterpret_cast<uint8_t*>(&value);
        for (int i = 0; i < sizeof(value); i++) {
            bytes.push_back(byte_arr[i]);
        }
        return bytes;
    }

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct StringConstant : Constant {
    std::string value;

    explicit StringConstant(std::string value) : Constant(ConstantType::String), value(std::move(value)) {}

    ByteList codegen() override {
        // TODO: UNICODE!!!!
        ByteList bytes = {type_code()};
        for (const auto & c : value) {
            bytes.push_back(static_cast<uint8_t>(c));
        }
        return bytes;
    }

    std::string to_string() override {
        return value;
    }
};

struct Chunk {
    ByteList code;
    std::vector<constant_ptr> constants;
//    std::vector<Attribute> attributes;
};

#endif
