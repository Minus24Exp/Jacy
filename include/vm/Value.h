#ifndef VALUE_H
#define VALUE_H

#include "compiler/opcode.h"
#include <string>
#include <functional>
#include <utility>
#include <sstream>
#include <iomanip>
#include <limits>

struct Value;
using value_ptr = std::shared_ptr<Value>;

struct Value {
    virtual std::string to_string() = 0;
};

struct NullValue : Value {
    std::string to_string() override {
        return "null";
    }
};
const auto Null = std::make_shared<NullValue>();

struct FalseValue : Value {
    std::string to_string() override {
        return "false";
    }
};
const auto False = std::make_shared<FalseValue>();

struct TrueValue : Value {
    std::string to_string() override {
        return "true";
    }
};
const auto True = std::make_shared<TrueValue>();

struct Int : Value {
    long long value;

    explicit Int(const std::shared_ptr<IntConstant> & int_constant) : value(int_constant->value) {}

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct Float : Value {
    double value;

    explicit Float(const std::shared_ptr<FloatConstant> & float_constant) : value(float_constant->value) {}

    std::string to_string() override {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
        return ss.str();
    }
};

struct String : Value {
    std::string value;

    explicit String(const std::shared_ptr<StringConstant> & string_constant) : value(string_constant->value) {}

    std::string to_string() override {
        return value;
    }
};

using FuncArgs = std::vector<value_ptr>;
struct Func : Value {

};

struct NativeFunc;
using nf_ptr = std::shared_ptr<NativeFunc>;
using NFBody = std::function<value_ptr(FuncArgs)>;
struct NativeFunc : Value {
    std::string name;
    NFBody body;

    explicit NativeFunc(std::string name, NFBody body) : name(std::move(name)), body(std::move(body)) {}

    std::string to_string() override {
        return "native_func_" + name;
    }
};

static inline nf_ptr make_nf(const std::string & name, const NFBody & body) {
    return std::make_shared<NativeFunc>(name, body);
}

#endif // VALUE_H
