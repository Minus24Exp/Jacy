#ifndef CONSTANT_H
#define CONSTANT_H

#include "bytecode/opcode.h"

// TODO: constant_from_code() function for VM
// TODO: Remove types from constants and create helpers for type determination, constants must not depend on compiler

namespace jc::bytecode {
    struct Constant {
        Constant() = default;

        virtual ByteList codegen() = 0;

        // Debug
        virtual std::string to_string() = 0;
    };

    struct IntConstant : Constant {
        explicit IntConstant(long long value) : value(value) {}

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
        explicit FloatConstant(double value) : value(value) {}

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
        explicit StringConstant(std::string value) : value(std::move(value)) {}

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

    struct FuncConstant : Constant {
        FuncConstant(uint32_t name_offset, uint32_t arg_count, uint32_t upvalue_count, ByteList code)
            : name_offset(name_offset), arg_count(arg_count), upvalue_count(upvalue_count), code(std::move(code)) {}

        uint32_t name_offset;
        uint32_t arg_count;
        uint32_t upvalue_count;
        ByteList code;

        ByteList codegen() override {
            // TODO
        }

        std::string to_string() override {
            // TODO:
            return "TODO: function";
        }
    };
}

#endif // CONSTANT_H
