#include "vm/BaseVM.h"

BaseVM::BaseVM() {
    for (const auto & global : jcGlobals) {
        globals[global.first] = global.second.value;
    }
}

//////////////
// Bytecode //
//////////////
uint8_t BaseVM::peek() const {
    return chunk.code[ip];
}

opcode_it BaseVM::peek_it() {
    return chunk.code.begin() + ip;
}

void BaseVM::advance(int distance) {
    ip += distance;
}

uint8_t BaseVM::read() {
    uint8_t b = peek();
    advance();
    return b;
}

uint16_t BaseVM::read2() {
    uint16_t s;
    std::copy(peek_it(), peek_it() + 2, reinterpret_cast<uint8_t*>(&s));
    advance(2);
    return s;
}

uint32_t BaseVM::read4() {
    uint32_t i;
    std::copy(peek_it(), peek_it() + 4, reinterpret_cast<uint8_t*>(&i));
    advance(4);
    return i;
}

uint64_t BaseVM::read8() {
    uint64_t l;
    std::copy(peek_it(), peek_it() + 8, reinterpret_cast<uint8_t*>(&l));
    advance(8);
    return l;
}

///////////
// Stack //
///////////
void BaseVM::push(const value_ptr & value) {
    stack.push_back(value);
}

value_ptr BaseVM::pop() {
    value_ptr back = stack.back();
    stack.pop_back();
    return back;
}

value_ptr BaseVM::top(uint64_t offset) {
    return stack.at(stack.size() - offset - 1);
}

///////////////
// Constants //
///////////////
constant_ptr BaseVM::read_const() {
    // Note: read_const automatically reads constant offset and advances
    const auto & offset = read8();
    if (offset >= chunk.constants.size()) {
        // TODO: Remove cout
        std::cout << std::endl << "offset: " << offset << ". size: " << chunk.constants.size() << std::endl;
        throw DevError("Constant offset is out of constant pool bounds");
    }
    return chunk.constants.at(offset);
}

std::shared_ptr<IntConstant> BaseVM::read_int_const() {
    constant_ptr constant = read_const();
    // TODO: Remove type checking
    if (constant->type != ConstantType::Int) {
        throw DevError("Invalid constant type, (int) expected");
    }
    return std::static_pointer_cast<IntConstant>(constant);
}

std::shared_ptr<FloatConstant> BaseVM::read_float_const() {
    constant_ptr constant = read_const();
    // TODO: Remove type checking
    if (constant->type != ConstantType::Float) {
        throw DevError("Invalid constant type, (float) expected");
    }
    return std::static_pointer_cast<FloatConstant>(constant);
}

std::shared_ptr<StringConstant> BaseVM::read_string_const() {
    constant_ptr constant = read_const();
    // TODO: Remove type checking
    if (constant->type != ConstantType::String) {
        throw DevError("Invalid constant type, (string) expected");
    }
    return std::static_pointer_cast<StringConstant>(constant);
}

std::vector<value_ptr> BaseVM::read_args(uint64_t arg_count) {
    std::vector<value_ptr> args;
    args.reserve(arg_count);
    for (uint64_t i = 0; i < arg_count; i++) {
        args.push_back(top(arg_count - i - 1));
    }
    return args;
}

////////////
// Errors //
////////////
void BaseVM::error(const std::string & msg) {
    // TODO: Use VM exceptions
    throw JacyException(msg);
}
