#ifndef VM_H
#define VM_H

#include "vm/BaseVM.h"
#include "vm/class.h"
#include "common/Logger.h"
#include "../../lib/globals.h"

namespace jc::vm {
    struct CallFrame {
        // Closure is not nullptr if we are in a function
        std::shared_ptr<Closure> closure{nullptr};
        std::vector<value_ptr> slots;
        uint64_t ip;
    };

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
        void _get_upvalue() override;
        void _set_upvalue() override;
        void _close_upvalue() override;
        void _closure() override;
        void _jump() override;
        void _jump_false() override;
        void _invoke() override;
        void _invoke_nf() override;
        void _invoke_method() override;
        void _invoke_nf_method() override;
        void _get_property() override;
        void _set_property() override;

        // Byte-code reading //
        bytecode::bytelist_it peek_it() override;
        void advance(int distance) override;

        // Stack //
        std::vector<object_ptr> stack;
        void push(const object_ptr & value);
        object_ptr pop();
        object_ptr top(uint32_t offset = 0);

        std::map<std::string, object_ptr> globals;

        // Call-frames //
        std::vector<CallFrame> call_frames;
        std::vector<CallFrame>::iterator frame;
        std::vector<object_ptr> read_args(uint32_t arg_count);

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
