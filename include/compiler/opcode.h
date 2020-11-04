#ifndef OPCODE_H
#define OPCODE_H

#include <utility>
#include <vector>
#include <cstdint>
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

using ByteList = std::vector<uint8_t>;

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
 * Opcode -- | -- Description -- | -- operands
 * NOP - "Nothing" opcode
 * NullConst - Null constant
 * FalseConst - False constant
 * TrueConst - True constant
 * StringConst - String constant | size (8), ...chars[size]
 */

enum class OpCode : uint8_t {
    NOP,

    NullConst,
    FalseConst,
    TrueConst,
    IntConst,
    FloatConst,
    StringConst,
};

enum class ConstantType : uint8_t {
    Int,
    Float,
    String,
};

struct Constant {
    ConstantType type;
    explicit Constant(ConstantType type) : type(type) {}

    uint8_t type_code() {
        return static_cast<uint8_t>(type);
    }
    virtual ByteList codegen() = 0;
};

struct IntConstant : Constant {
    long long value;

    explicit IntConstant(long long value) : Constant(ConstantType::Int), value(value) {}

    ByteList codegen() override {
        ByteList bytes = {type_code()};
        for (int i = 0; i < sizeof(value); i++) {
            bytes.push_back((value >> (8u * i)) & 0XFF);
        }
        return bytes;
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
};

struct Chunk {
    std::vector<Constant> constants;
    std::vector<uint8_t> code;
//    std::vector<Attribute> attributes;
};

#endif
