#ifndef CONSTANT_H
#define CONSTANT_H

#include "bytecode/opcode.h"
#include "compiler/type.h"

// TODO: constant_from_code() function for VM
// TODO: Remove types from constants and create helpers for type determination, constants must not depend on compiler

namespace jc::bytecode {
    struct Constant {
        // Constant receive type only on Compile-time, separate VM must use stub for compiler::type_ptr
        explicit Constant(const compiler::type_ptr & type) : type(type) {}

        compiler::type_ptr type;

        virtual ByteList codegen() = 0;

        // Debug
        virtual std::string to_string() = 0;
    };

    struct IntConstant : Constant {
        explicit IntConstant(long long value, const compiler::type_ptr & int_t = nullptr)
            : Constant(int_t), value(value) {}

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
        explicit FloatConstant(double value, const compiler::type_ptr & float_t = nullptr)
            : Constant(float_t), value(value) {}

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
        explicit StringConstant(std::string value, const compiler::type_ptr & string_t = nullptr)
            : Constant(string_t), value(std::move(value)) {}

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
}

#endif // CONSTANT_H
