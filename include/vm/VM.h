#ifndef VM_H
#define VM_H

#include "vm/BaseVM.h"
#include "vm/Value.h"
#include <stack>
#include <map>

class VM : public BaseVM {
public:
    VM();
    virtual ~VM() = default;

private:
    std::stack<Value> stack;
    std::vector<Value> consts;
    std::vector<Value> slots;

    void push(Value val);
    Value top();

    void load_const(uint8_t offset) override;
    void load_var(uint64_t offset) override;
    void store_var(uint64_t offset) override;
};

#endif
