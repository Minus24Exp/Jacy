#ifndef CONSTANT_H
#define CONSTANT_H

#include "bytecode/opcode.h"

// TODO: constant_from_code() function for VM
// TODO: Remove types from constants and create helpers for type determination, constants must not depend on compiler

namespace jc::bytecode {
    enum class ConstTag {
        Int,
        Float,
        String,
        Func,
    };

    struct Constant {
        explicit Constant(ConstTag tag) : tag(tag) {}

        ConstTag tag;

        virtual byte_list codegen() = 0;

        // Debug
        virtual std::string to_string() = 0;
    };

    struct IntConstant : Constant {
        explicit IntConstant(long long value) : Constant(ConstTag::Int), value(value) {}

        long long value;

        byte_list codegen() override {
    //        byte_list bytes = {type_code()};
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
        explicit FloatConstant(double value) : Constant(ConstTag::Float), value(value) {}

        double value;

        byte_list codegen() override {
//        byte_list bytes = {type_code()};
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
        explicit StringConstant(std::string value) : Constant(ConstTag::String), value(std::move(value)) {}

        std::string value;

        byte_list codegen() override {
            // TODO: UNICODE!!!!
//        byte_list bytes = {type_code()};
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
        FuncConstant(uint32_t name_offset, uint32_t param_count)
            : Constant(ConstTag::Func), name_offset(name_offset), param_count(param_count) {}

        uint32_t name_offset;
        uint32_t param_count;
        uint32_t upvalue_count{0};
        byte_list code{};

        byte_list codegen() override {
            // TODO
        }

        std::string to_string() override {
            return "[Function stub]";
        }
    };
}

#endif // CONSTANT_H
