#include "vm/VM.h"

namespace jc::vm {
    VM::VM() : log("VM", options.log) {
        for (const auto & global : globals::jcGlobals) {
            globals[global.first] = global.second.value;
        }

        call_frames.push_back({});
        frame = call_frames.begin();
    }

    void VM::_nop() {}

    void VM::_pop() {
        pop();
    }

    void VM::_null_const() {
        push(Null);
    }

    void VM::_false_const() {
        push(False);
    }

    void VM::_true_const() {
        push(True);
    }

    void VM::_int_const() {
        const auto & int_const = read_int_const();
        push(std::make_shared<IntObject>(int_const));
    }

    void VM::_float_const() {
        const auto & float_const = read_float_const();
        push(std::make_shared<FloatObject>(float_const));
    }

    void VM::_string_const() {
        const auto & string_const = read_string_const();
        push(std::make_shared<StringObject>(string_const));
    }

    void VM::_define_global() {
        const auto & global_name = read_string_const();
        globals.at(global_name->value) = nullptr;
    }

    void VM::_load_global() {
        const auto & global_name = read_string_const();
        try {
            push(globals.at(global_name->value));
        } catch (std::out_of_range & e) {
            error(global_name->value + " is not defined");
        }
    }

    void VM::_store_global() {
        const auto & global_name = read_string_const();
        try {
            globals.at(global_name->value) = top();
        } catch (std::out_of_range & e) {
            error(global_name->value + " is not defined");
        }
    }

    void VM::_load_local() {
        const auto & slot = read8();
        try {
            push(frame->slots.at(slot)->object);
        } catch (std::out_of_range & e) {
            // TODO: Update when locals names will be done
            error("Unable to resolve local");
        }
    }

    void VM::_store_local() {
        const auto & slot = read8();
        try {
            frame->slots.at(slot)->object = top();
        } catch (std::out_of_range & e) {
            error("Unable to resolve local");
        }
    }

    void VM::_get_upvalue() {
        uint32_t slot = read4();
        std::cout << slot;
    }

    void VM::_set_upvalue() {
        uint32_t slot = read4();
        std::cout << slot;
    }

    void VM::_close_upvalue() {

    }

    void VM::_closure() {

    }

    void VM::_jump() {
        const auto & offset = read8();
        frame->ip += offset;
    }

    void VM::_jump_false() {
        const auto & offset = read8();
        const auto & object = top();
        bool truthy = false;
        if (!is_instance_obj(object->type)) {
            // Note: All non-instance objects considered truthy
            truthy = true;
        } else {
            // TODO: Use overridden language-level to_b not instance.to_b, so use invoke(instance, 'to_b')
            truthy = as_instance(object)->to_b();
        }
        if (truthy) {
            frame->ip += offset;
        }
    }

    void VM::_invoke() {
        uint32_t arg_count = read4();
        invoke_object(top(arg_count), arg_count);
        frame = call_frames.end() - 1;
    }

    void VM::_invoke_nf() {
        uint32_t arg_count = read8();
        // TODO: Verify
        const auto & func = std::static_pointer_cast<NativeFunc>(top(arg_count));
        const auto & args = read_args(arg_count);
        auto value = func->body(args);
        // TODO: !!! Create `void`, DO NOT USE NULL
        //  Void is not about run-time, so it must be caught on compile-time and push nothing on run-time
        value = value ? value : Null;
        push(value);
    }

    void VM::_invoke_method() {
        // TODO
    }

    void VM::_invoke_nf_method() {
        uint32_t arg_count = read8();
        const auto & method_name = read_string_const()->value;
        const auto & instance = as_instance(top(arg_count));
        const auto & args = read_args(arg_count);
        const auto & method = std::static_pointer_cast<NativeFunc>(instance->_class->methods.at(method_name));
        auto value = method->body(args);
        // TODO: Look above (about void)
        value = value ? value : Null;
        push(value);
    }

    void VM::_get_property() {
        const auto & instance = as_instance(top());
        const auto & prop_name = read_string_const();
        pop();
        push(instance->fields.at(prop_name->value));
    }

    void VM::_set_property() {
        // TODO
    }

    // Byte-code reading //
    bytecode::bytelist_it VM::peek_it() {
        if (frame->closure) {
            return frame->closure->func->code.begin() + frame->ip;
        }
        return chunk.code.begin() + frame->ip;
    }

    void VM::advance(int distance) {
        frame->ip += distance;
    }

    ///////////
    // Stack //
    ///////////
    void VM::push(const object_ptr & object) {
        stack.push_back(std::make_shared<Value>(object));
    }

    object_ptr VM::pop() {
        object_ptr back = stack.back()->object;
        stack.pop_back();
        return back;
    }

    object_ptr VM::top(uint32_t offset) {
        return stack.at(stack.size() - offset - 1)->object;
    }

    std::vector<object_ptr> VM::read_args(uint32_t arg_count) {
        std::vector<object_ptr> args;
        args.reserve(arg_count);
        for (uint32_t i = 0; i < arg_count; i++) {
            args.push_back(top(arg_count - i - 1));
        }
        return args;
    }

    std::shared_ptr<Instance> VM::as_instance(const object_ptr & object) {
        if (!is_instance_obj(object->type)) {
            throw DevError("Instance object expected");
        }
        return std::static_pointer_cast<Instance>(object);
    }

    void VM::invoke_object(object_ptr callee, uint32_t arg_count) {
        switch (callee->type) {
            case ObjectType::Closure: {
                invoke(std::static_pointer_cast<Closure>(callee), arg_count);
            } break;
            case ObjectType::NativeFunc: {
                // TODO
            } break;
            case ObjectType::Class: {
                // TODO
            } break;
            default: {
                invalid_bytecode("Non-callable object in function call");
            }
        }
    }

    void VM::invoke(closure_ptr closure, uint32_t arg_count) {
        // TODO!: Store args count in function object and verify

        if (call_frames.size() >= CallFrame::FRAMES_LIMIT) {
            error("Stack overflow");
        }

        call_frames.push_back({closure});
        frame = call_frames.end() - 1;
        frame->slots = std::vector<value_ptr>(stack.end() - arg_count - 1, stack.end());
    }

    ////////////
    // Errors //
    ////////////
    void VM::error(const std::string & msg) {
        // TODO: Current file
        // TODO: Position tracking
        throw RuntimeError(msg, {});
    }

    void VM::invalid_bytecode(const std::string & msg) {
        // TODO: Move to verifier
        throw ByteCodeVerificationError(msg);
    }
}
