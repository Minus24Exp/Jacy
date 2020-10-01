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
    std::vector<Value> slots;

    void push(Value val);
    Value top();

    void load_null() override;
    void load_bool(bool value) override;
    void load_int(int64_t value) override;
    void load_float(double value) override;
    void load_string(const char * value) override;

    void load_var(uint64_t offset) override;
    void store_var(uint64_t offset) override;
};

#endif
