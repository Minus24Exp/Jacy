#include "vm/object.h"

namespace jc::vm {
    ////////////////
    // NullObject //
    ////////////////
    NullObject::NullObject() {

    }

    bool NullObject::to_b() {
        return false;
    }

    std::string NullObject::to_string() {
        return "null";
    }

    /////////////////
    // FalseObject //
    /////////////////
    FalseObject::FalseObject() {

    }

    bool FalseObject::to_b() {
        return false;
    }

    std::string FalseObject::to_string() {
        return "false";
    }

    ////////////////
    // TrueObject //
    ////////////////
    TrueObject::TrueObject() {

    }

    bool TrueObject::to_b() {
        return true;
    }

    std::string TrueObject::to_string() {
        return "true";
    }

    ///////////////
    // IntObject //
    ///////////////
    IntObject::IntObject(long long value) : value(value) {}

    IntObject::IntObject(const std::shared_ptr<bytecode::IntConstant> & int_constant)
        : IntObject(int_constant->value) {}

    bool IntObject::to_b() {
        return value != 0L;
    }

    std::string IntObject::to_string() {
        return std::to_string(value);
    }

    /////////////////
    // FloatObject //
    /////////////////
    FloatObject::FloatObject(const std::shared_ptr<bytecode::FloatConstant> &float_constant)
        : value(float_constant->value) {}

    bool FloatObject::to_b() {
        return value != 0.0F;
    }

    std::string FloatObject::to_string() {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
        return ss.str();
    }

    //////////////////
    // StringObject //
    //////////////////
    StringObject::StringObject(const std::string & value) : value(value) {
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

    //////////
    // Func //
    //////////
    Func::Func(std::string name) : name(std::move(name)) {}

    bool Func::to_b() {
        return true;
    }

    std::string Func::to_string() {
        return "<func:"+ name +">";
    }

    ////////////////
    // NativeFunc //
    ////////////////
    NativeFunc::NativeFunc(std::string name, NFBody body) : name(std::move(name)), body(std::move(body)) {}

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
