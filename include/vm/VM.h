#ifndef VM_H
#define VM_H

#include "vm/BaseVM.h"
#include "common/Logger.h"

namespace jc::vm {
    class VM : public BaseVM {
    public:
        VM();
        ~VM() override = default;

    //    void before_eval() override;
    //    void before_opcode(OpCode opcode) override;
    //    void after_opcode() override;
    //    void unknown_opcode() override;
        void _nop() override;
        void _pop() override;
        void _null_const() override;
        void _false_const() override;
        void _true_const() override;
        void _int_const() override;
        void _float_const() override;
        void _string_const() override;
        void _define_global() override;
        void _load_global() override;
        void _store_global() override;
        void _load_local() override;
        void _store_local() override;
        void _jump() override;
        void _jump_false() override;
        void _invoke() override;
        void _invoke_nf() override;
        void _invoke_method() override;
        void _invoke_nf_method() override;
        void _get_property() override;
        void _set_property() override;

    private:
        // Errors //
        static void error(const std::string & msg);

        // DEBUG //
    public:
        common::VMOptions options;
        common::Logger log;
    };
}

#endif // VM_H
