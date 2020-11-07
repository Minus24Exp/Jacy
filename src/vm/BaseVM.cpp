#include "vm/BaseVM.h"

void BaseVM::push(const Value & value) {
    stack.push(value);
}

Value BaseVM::pop() {
    Value back = stack.top();
    stack.pop();
    return back;
}
