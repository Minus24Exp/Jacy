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
    struct Closure;
    struct Upvalue;
    struct Value;

    using closure_ptr = std::shared_ptr<Closure>;
    using upvalue_ptr = std::shared_ptr<Upvalue>;
    using value_ptr = std::shared_ptr<Value>;
    using object_ptr = std::shared_ptr<Object>;
    using class_ptr = std::shared_ptr<Class>;
    using FuncArgs = std::vector<object_ptr>;
    using func_ptr = std::shared_ptr<Func>;
    using nf_ptr = std::shared_ptr<NativeFunc>;
    using NFBody = std::function<object_ptr(const FuncArgs&)>;

    enum class ObjectType : uint8_t {
        // Internal-level
        Upvalue     = 0b00001,
        Closure     = 0b00010,
        Class       = 0b00011,

        // Language-level
        Instance    = 0b10001,
        Unit        = 0b10010,
        Null        = 0b10011,
        Bool        = 0b10100,
        Int         = 0b10101,
        Float       = 0b10110,
        String      = 0b10111,
        Func        = 0b11000,
        NativeFunc  = 0b11001,
    };

    bool is_instance_obj(ObjectType type);

    struct Value {
        explicit Value(object_ptr object) : object(object) {}

        object_ptr object;
    };

    struct Object {
        explicit Object(ObjectType type);

        ObjectType type;
    };

    struct Upvalue : Object {
        Upvalue();

        value_ptr location;
        Value closed{nullptr};
        std::shared_ptr<Upvalue> next;
    };

    struct Closure : Object {
        explicit Closure(func_ptr func);

        func_ptr func;
        std::vector<upvalue_ptr> upvalues{};
    };

    struct Instance : Object {
        explicit Instance(ObjectType type = ObjectType::Instance);

        class_ptr _class;
        std::map<std::string, object_ptr> fields;

        virtual bool to_b() = 0;
        virtual std::string to_string() = 0;
    };

    struct Unit : Instance {
        explicit Unit();

        bool to_b() override;
        std::string to_string() override;
    };

    struct NullObject : Instance {
        NullObject();

        bool to_b() override;
        std::string to_string() override;
    };
    const auto Null = std::make_shared<NullObject>();

    struct FalseObject : Instance {
        FalseObject();

        bool to_b() override;
        std::string to_string() override;
    };
    const auto False = std::make_shared<FalseObject>();

    struct TrueObject : Instance {
        TrueObject();

        bool to_b() override;
        std::string to_string() override;
    };
    const auto True = std::make_shared<TrueObject>();

    struct IntObject : Instance {
        explicit IntObject(long long value);
        explicit IntObject(const std::shared_ptr<bytecode::IntConstant> & int_constant);

        long long value;

        bool to_b() override;
        std::string to_string() override;
    };

    struct FloatObject : Instance {
        explicit FloatObject(const std::shared_ptr<bytecode::FloatConstant> & float_constant);

        double value;

        bool to_b() override;
        std::string to_string() override;
    };

    struct StringObject : Instance {
        explicit StringObject(const std::string & value);
        explicit StringObject(const std::shared_ptr<bytecode::StringConstant> & string_constant);

        std::string value;

        bool to_b() override;
        std::string to_string() override;
    };

    // TODO: Function
    struct Func : Instance {
        explicit Func(std::string name, bytecode::byte_list code);

        std::string name;
        bytecode::byte_list code;

        bool to_b() override;
        std::string to_string() override;
    };

    struct NativeFunc : Instance {
        explicit NativeFunc(std::string name, NFBody body);

        std::string name;
        NFBody body;

        bool to_b() override;
        std::string to_string() override;
    };

    nf_ptr make_nf(const std::string & name, const NFBody & body);
}

#endif // OBJECT_H
