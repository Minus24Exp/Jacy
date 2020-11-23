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
    struct Callable;
    struct Func;
    struct NativeFunc;

    using object_ptr = std::shared_ptr<Object>;
    using class_ptr = std::shared_ptr<Class>;
    using FuncArgs = std::vector<object_ptr>;
    using func_ptr = std::shared_ptr<Func>;
    using nf_ptr = std::shared_ptr<NativeFunc>;
    using callable_ptr = std::shared_ptr<Callable>;
    using NFBody = std::function<object_ptr(const FuncArgs&)>;

    struct Object {
        class_ptr _class;
        std::map<std::string, object_ptr> fields;

        virtual bool to_b() = 0;
        virtual std::string to_string() = 0;
    };

    struct NullObject : Object {
        NullObject();

        bool to_b() override;

        std::string to_string() override;
    };
    const auto Null = std::make_shared<NullObject>();

    struct FalseObject : Object {
        FalseObject();

        bool to_b() override;

        std::string to_string() override;
    };
    const auto False = std::make_shared<FalseObject>();

    struct TrueObject : Object {
        TrueObject();

        bool to_b() override;

        std::string to_string() override;
    };
    const auto True = std::make_shared<TrueObject>();

    struct IntObject : Object {
        explicit IntObject(long long value);
        explicit IntObject(const std::shared_ptr<bytecode::IntConstant> & int_constant);

        long long value;

        bool to_b() override;

        std::string to_string() override;
    };

    struct FloatObject : Object {
        explicit FloatObject(const std::shared_ptr<bytecode::FloatConstant> & float_constant);

        double value;

        bool to_b() override;

        std::string to_string() override;
    };

    struct StringObject : Object {
        explicit StringObject(const std::string & value);
        explicit StringObject(const std::shared_ptr<bytecode::StringConstant> & string_constant);

        std::string value;

        bool to_b() override;

        std::string to_string() override;
    };

    struct Callable : Object {};

    // TODO: Function
    struct Func : Callable {
        explicit Func(std::string name);

        std::string name;
        // TODO: Body

        bool to_b() override;

        std::string to_string() override;
    };

    struct NativeFunc : Callable {
        explicit NativeFunc(std::string name, NFBody body);

        std::string name;
        NFBody body;

        bool to_b() override;

        std::string to_string() override;
    };

    nf_ptr make_nf(const std::string & name, const NFBody & body);
}

#endif // OBJECT_H
