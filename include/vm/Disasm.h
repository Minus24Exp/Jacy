#ifndef DISASM_H
#define DISASM_H

#include "vm/BaseVM.h"

class Disasm : public BaseVM {
public:
    Disasm();
    ~Disasm() override = default;

    void _pop() override;
    void null_const() override;
    void false_const() override;
    void true_const() override;
    void int_const() override;
    void float_const() override;
    void string_const() override;
    void define_global() override;
    void load_global() override;
    void store_global() override;
    void load_local() override;
    void store_local() override;
    void jump() override;
    void jump_false() override;
    void call() override;
    void get_property() override;
    void set_property() override;
};

#endif // DISASM_H
