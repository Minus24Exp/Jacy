#ifndef VALUE_H
#define VALUE_H

#include "compiler/opcode.h"
#include "compiler/class.h"
#include "compiler/constant.h"

#include <string>
#include <functional>
#include <utility>
#include <sstream>
#include <iomanip>
#include <limits>
#include <map>

struct Object;
using object_ptr = std::shared_ptr<Object>;

struct NativeFunc;
using FuncArgs = std::vector<object_ptr>;
using nf_ptr = std::shared_ptr<NativeFunc>;
using NFBody = std::function<object_ptr(FuncArgs)>;

struct Object {
//    class_ptr _class;
//    std::map<std::string, Field> fields;

    virtual bool to_b() = 0;
    virtual std::string to_string() = 0;

//    bool has_field(const std::string & name) {
//        return fields.find(name) != fields.end() || _class->methods.find(name) != _class->methods.end();
//    }

//     TODO
//     value_ptr get();
};

struct NullObject : Object {
    bool to_b() override {
        return false;
    }

    std::string to_string() override {
        return "null";
    }
};
const auto Null = std::make_shared<NullObject>();

struct FalseObject : Object {
    bool to_b() override {
        return false;
    }

    std::string to_string() override {
        return "false";
    }
};
const auto False = std::make_shared<FalseObject>();

struct TrueObject : Object {
    bool to_b() override {
        return true;
    }

    std::string to_string() override {
        return "true";
    }
};
const auto True = std::make_shared<TrueObject>();

struct IntObject : Object {
    explicit IntObject(const std::shared_ptr<IntConstant> & int_constant) : value(int_constant->value) {}

    long long value;

    bool to_b() override {
        return value != 0L;
    }

    std::string to_string() override {
        return std::to_string(value);
    }
};

struct FloatObject : Object {
    explicit FloatObject(const std::shared_ptr<FloatConstant> & float_constant) : value(float_constant->value) {}

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

struct StringObject : Object {
    explicit StringObject(const std::shared_ptr<StringConstant> & string_constant) : value(string_constant->value) {}

    std::string value;

    bool to_b() override {
        return !value.empty();
    }

    std::string to_string() override {
        return value;
    }
};

struct NativeFunc : Object {
    explicit NativeFunc(std::string name, NFBody body) : name(std::move(name)), body(std::move(body)) {}

    std::string name;
    NFBody body;

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
