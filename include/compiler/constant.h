#ifndef CONSTANT_H
#define CONSTANT_H

#include "compiler/opcode.h"
#include "compiler/type.h"

struct Constant {
    explicit Constant(const type_ptr & type) : type(type) {}

    type_ptr type;

    virtual ByteList codegen() = 0;

    // Debug
    virtual std::string to_string() = 0;
};

struct IntConstant : Constant {
    explicit IntConstant(long long value) : Constant(int_t), value(value) {}

    long long value;

    ByteList codegen() override {
//        ByteList bytes = {type_code()};
//        for (int i = 0; i < sizeof(value); i++) {
//            bytes.push_back((value >> (i * U8)) & U255);
//        }
//        return bytes;
    }

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct FloatConstant : Constant {
    explicit FloatConstant(double value) : Constant(float_t), value(value) {}

    double value;

    ByteList codegen() override {
//        ByteList bytes = {type_code()};
//        const auto * byte_arr = reinterpret_cast<uint8_t*>(&value);
//        for (int i = 0; i < sizeof(value); i++) {
//            bytes.push_back(byte_arr[i]);
//        }
//        return bytes;
    }

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct StringConstant : Constant {
    explicit StringConstant(std::string value) : Constant(string_t), value(std::move(value)) {}

    std::string value;

    ByteList codegen() override {
        // TODO: UNICODE!!!!
//        ByteList bytes = {type_code()};
//        for (const auto & c : value) {
//            bytes.push_back(static_cast<uint8_t>(c));
//        }
//        return bytes;
    }

    std::string to_string() override {
        return value;
    }
};

#endif // CONSTANT_H
