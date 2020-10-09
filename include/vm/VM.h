#ifndef VM_H
#define VM_H

#include <stack>
#include <map>
#include "vm/BaseVM.h"
#include "vm/Value.h"
#include "object/Objects.h"

class VM : public BaseVM {
public:
    VM();
    virtual ~VM() = default;

private:
    std::stack<Value> stack;
    std::vector<Value> slots;
    std::map<std::string, Value> globals;

    void push(const Value & val);
    Value top();
    Value pop_stack();

    void load_null() override;
    void load_bool(bool value) override;
    void load_int(int64_t value) override;
    void load_float(double value) override;
    void load_string(const char * value) override;
    void load_var(uint64_t offset) override;
    void store_var(uint64_t offset) override;
    void pop() override;
    void call(uint8_t args_count) override;
};

#endif
