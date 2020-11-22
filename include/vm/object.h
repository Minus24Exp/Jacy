#ifndef OBJECT_H
#define OBJECT_H

#include "bytecode/constant.h"

#include <string>
#include <functional>
#include <utility>
#include <sstream>
#include <iomanip>
#include <limits>
#include <map>

namespace jc::vm {
    struct Object;
    struct Class;
    struct Func;
    struct NativeFunc;

    using object_ptr = std::shared_ptr<Object>;
    using class_ptr = std::shared_ptr<Class>;
    using FuncArgs = std::vector<object_ptr>;
    using func_ptr = std::shared_ptr<Func>;
    using nf_ptr = std::shared_ptr<NativeFunc>;
    using NFBody = std::function<object_ptr(FuncArgs)>;

    struct Object {
        class_ptr _class;
        std::map<std::string, object_ptr> fields;

        virtual bool to_b() = 0;
        virtual std::string to_string() = 0;
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
        explicit IntObject(long long value) : value(value) {}
        explicit IntObject(const std::shared_ptr<bytecode::IntConstant> & int_constant) : IntObject(int_constant->value) {}

        long long value;

        bool to_b() override {
            return value != 0L;
        }

        std::string to_string() override {
            return std::to_string(value);
        }
    };

    struct FloatObject : Object {
        explicit FloatObject(const std::shared_ptr<bytecode::FloatConstant> & float_constant) : value(float_constant->value) {}

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
        explicit StringObject(const std::string & value) : value(value) {
            // TODO: Remove everything from here
            fields.insert({"size", std::make_shared<IntObject>(value.size())});
        }
        explicit StringObject(const std::shared_ptr<bytecode::StringConstant> & string_constant) : StringObject(string_constant->value) {}

        std::string value;

        bool to_b() override {
            return !value.empty();
        }

        std::string to_string() override {
            return value;
        }
    };

    // TODO: Function
    struct Func : Object {
        explicit Func(std::string name) : name(std::move(name)) {}

        std::string name;
        // TODO: Body

        bool to_b() override {
            return true;
        }

        std::string to_string() override {
            return "<func:"+ name +">";
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
}

#endif // OBJECT_H
