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
struct Object;
using object_ptr = std::shared_ptr<Object>;
using value_ptr = std::shared_ptr<Value>;

struct Value {
    virtual bool to_b() = 0;
    virtual std::string to_string() = 0;

    object_ptr object;
};

struct NullValue : Value {
    bool to_b() override {
        return false;
    }

    std::string to_string() override {
        return "null";
    }
};
const auto Null = std::make_shared<NullValue>();

struct FalseValue : Value {
    bool to_b() override {
        return false;
    }

    std::string to_string() override {
        return "false";
    }
};
const auto False = std::make_shared<FalseValue>();

struct TrueValue : Value {
    bool to_b() override {
        return true;
    }

    std::string to_string() override {
        return "true";
    }
};
const auto True = std::make_shared<TrueValue>();

struct Int : Value {
    explicit Int(const std::shared_ptr<IntConstant> & int_constant) : value(int_constant->value) {}

    long long value;

    bool to_b() override {
        return value != 0L;
    }

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct Float : Value {
    explicit Float(const std::shared_ptr<FloatConstant> & float_constant) : value(float_constant->value) {}

    double value;

    bool to_b() override {
        return value != 0.0F;
    }

    std::string to_string() override {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
        return ss.str();
    }
};

struct String : Value {
    explicit String(const std::shared_ptr<StringConstant> & string_constant) : value(string_constant->value) {}

    std::string value;

    bool to_b() override {
        return !value.empty();
    }

    std::string to_string() override {
        return value;
    }
};

struct NativeFunc;
using FuncArgs = std::vector<value_ptr>;
using nf_ptr = std::shared_ptr<NativeFunc>;
using NFBody = std::function<value_ptr(FuncArgs)>;
struct NativeFunc : Value {
    std::string name;
    NFBody body;

    explicit NativeFunc(std::string name, NFBody body) : name(std::move(name)), body(std::move(body)) {}

    bool to_b() override {
        return true;
    }

    std::string to_string() override {
        return "<native_func_" + name + ">";
    }
};

static inline nf_ptr make_nf(const std::string & name, const NFBody & body) {
    return std::make_shared<NativeFunc>(name, body);
}

#endif // VALUE_H
