#ifndef VM_H
#define VM_H

#include "vm/BaseVM.h"
#include "vm/Value.h"
#include <stack>

class VM : public BaseVM {
public:
    VM();
    virtual ~VM() = default;

private:
    std::stack<Value> stack;

    void push(Value val);

    void const_null() override;
    void const_bool(bool value) override;
    void const_int(yo_int value) override;
    void const_float(double value) override;
    void const_string(const std::string & value) override;
    void load(uint64_t offset) override;
    void store(uint64_t offset) override;
};

#endif
