#include "vm/class.h"

namespace jc::vm {
    bool is_instance_obj(ObjectType type) {
        return (static_cast<uint8_t>(type) >> 4u) & 1u;
    }

    Object::Object(ObjectType type) : type(type) {}

    // Upvalue //
    Upvalue::Upvalue() : Object(ObjectType::Upvalue) {}

    // Closure //
    Closure::Closure(func_ptr func) : Object(ObjectType::Closure), func(func) {}

    // Instance //
    Instance::Instance(ObjectType type) : Object(type) {}

    // NullObject //
    NullObject::NullObject() : Instance(ObjectType::Null) {}

    bool NullObject::to_b() {
        return false;
    }

    std::string NullObject::to_string() {
        return "null";
    }

    // FalseObject //
    FalseObject::FalseObject() : Instance(ObjectType::Bool) {}

    bool FalseObject::to_b() {
        return false;
    }

    std::string FalseObject::to_string() {
        return "false";
    }

    // TrueObject //
    TrueObject::TrueObject() : Instance(ObjectType::Bool) {}

    bool TrueObject::to_b() {
        return true;
    }

    std::string TrueObject::to_string() {
        return "true";
    }

    // IntObject //
    IntObject::IntObject(long long value) : Instance(ObjectType::Int), value(value) {
        // TODO!: Use class
        // TODO!: Use mangling functions
        // TODO!: This is the cause why I need API...
//        _class->methods.insert({
//            "_J[op]Int(Int)",
//            make_nf("_J[op]Int(Int)", [&](const FuncArgs & args) {
//                return std::make_shared<IntObject>(value + std::static_pointer_cast<IntObject>(args.at(0))->value);
//            })
//        });
    }

    IntObject::IntObject(const std::shared_ptr<bytecode::IntConstant> & int_constant)
        : IntObject(int_constant->value) {}

    bool IntObject::to_b() {
        return value != 0L;
    }

    std::string IntObject::to_string() {
        return std::to_string(value);
    }

    // FloatObject //
    FloatObject::FloatObject(const std::shared_ptr<bytecode::FloatConstant> &float_constant)
        : Instance(ObjectType::Float), value(float_constant->value) {}

    bool FloatObject::to_b() {
        return value != 0.0F;
    }

    std::string FloatObject::to_string() {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
        return ss.str();
    }

    // StringObject //
    StringObject::StringObject(const std::string & value) : Instance(ObjectType::String), value(value) {
        // TODO: Remove everything from here
        fields.insert({"size", std::make_shared<IntObject>(value.size())});
    }

    StringObject::StringObject(const std::shared_ptr<bytecode::StringConstant> &string_constant)
        : StringObject(string_constant->value) {}

    bool StringObject::to_b() {
        return !value.empty();
    }

    std::string StringObject::to_string() {
        return value;
    }

    // Func //
    Func::Func(std::string name, bytecode::byte_list code)
        : Instance(ObjectType::Func), name(std::move(name)), code(std::move(code)) {}

    bool Func::to_b() {
        return true;
    }

    std::string Func::to_string() {
        return "<func:"+ name +">";
    }

    // NativeFunc //
    NativeFunc::NativeFunc(std::string name, NFBody body)
        : Instance(ObjectType::NativeFunc), name(std::move(name)), body(std::move(body)) {}

    bool NativeFunc::to_b() {
        return true;
    }

    std::string NativeFunc::to_string() {
        return "<native_func_" + name + ">";
    }

    nf_ptr make_nf(const std::string & name, const NFBody & body) {
        return std::make_shared<NativeFunc>(name, body);
    }
}
