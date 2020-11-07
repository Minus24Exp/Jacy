#ifndef BASEVM_H
#define BASEVM_H

#include "vm/Value.h"
#include "compiler/opcode.h"
#include <stack>
#include <map>

class BaseVM {
public:
    BaseVM() = default;
    virtual ~BaseVM() = default;

    virtual void eval(const Chunk & chunk) = 0;

//    virtual void nop() = 0;
    virtual void _pop() = 0;
    virtual void null_const() = 0;
    virtual void false_const() = 0;
    virtual void true_const() = 0;
    virtual void int_const() = 0;
    virtual void float_const() = 0;
    virtual void string_const() = 0;
    virtual void define_global() = 0;
    virtual void load_global() = 0;
    virtual void store_global() = 0;
    virtual void load_local() = 0;
    virtual void store_local() = 0;
    virtual void jump() = 0;
    virtual void jump_false() = 0;
    virtual void call() = 0;
    virtual void get_property() = 0;
    virtual void set_property() = 0;

private:
    Chunk chunk;
    std::map<std::string, Value> globals;

    // Stack
    std::stack<Value> stack;
    void push(const Value & value);
    Value pop();
};

#endif // BASEVM_H
