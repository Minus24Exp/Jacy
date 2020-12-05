#include "compiler/Compiler.h"

namespace jc::compiler {
    Compiler::Compiler() : scope_depth(0), log("Compiler", options.log) {
        for (const auto & g : globals::jcGlobals) {
            globals[g.first] = std::make_shared<Variable>(tree::VarDeclKind::Val, g.second.type);
        }
    }

    bytecode::Chunk Compiler::compile(const tree::StmtList & tree) {
        enter_scope();
        scope_depth = 0;

        for (const auto & stmt : tree) {
            stmt->accept(*this);
        }

        return chunk;
    }

    ////////////////
    // Statements //
    ////////////////
    void Compiler::visit(tree::ExprStmt * expr_stmt) {
        expr_stmt->expr->accept(*this);
    }

    void Compiler::visit(tree::Block * block) {
        enter_scope();
        for (const auto & stmt : block->stmts) {
            stmt->accept(*this);
        }
        exit_scope();
    }

    void Compiler::visit(tree::VarDecl * var_decl) {
        tree::VarDeclKind kind = var_decl->kind;

        // TODO: Add real types (now any)
        type_ptr type = Any::get();

        const auto & var_name = var_decl->id->get_name();

        if (scope_depth == 0) {
            const auto & existent = globals.find(var_name);
            if (existent != globals.end()) {
                error("Unable to redefine global " + var_name, var_decl->pos);
            }

            // Define global
            uint64_t global = make_string(var_name);
            emit(bytecode::OpCode::DefineGlobal);
            emit(static_cast<uint64_t>(global));

            // TODO: ! No val variables without assign_expr and explicit type !
            globals[var_name] = std::make_shared<Variable>(kind, type);

            if (var_decl->assign_expr) {
                var_decl->assign_expr->accept(*this);
                emit(bytecode::OpCode::StoreGlobal);
                emit(static_cast<uint64_t>(global));

                globals[var_name]->is_defined = true;
            }
        } else {
            declare_var(kind, type, var_decl->id.get());
            if (var_decl->assign_expr) {
                // TODO: Extract type of expression
                var_decl->assign_expr->accept(*this);
                emit(bytecode::OpCode::StoreLocal);
                emit(static_cast<uint64_t>(scope->locals.size() - 1));
                scope->locals.back().is_defined = true;
                scope->locals.back().depth = scope_depth;
            }
        }
    }

    void Compiler::visit(tree::FuncDecl * func_decl) {

    //
    //    last_type = std::make_shared<FuncType>(return_type, arg_types);
    }

    void Compiler::visit(tree::ReturnStmt * expr_stmt) {

    }

    void Compiler::visit(tree::WhileStmt * expr_stmt) {

    }

    void Compiler::visit(tree::ForStmt * expr_stmt) {

    }

    void Compiler::visit(tree::ClassDecl * class_decl) {
    }

    void Compiler::visit(tree::Import * expr_stmt) {

    }

    void Compiler::visit(tree::TypeDecl * expr_stmt) {

    }

    /////////////////
    // Expressions //
    /////////////////
    void Compiler::visit(tree::Grouping * grouping) {
        grouping->expr->accept(*this);
    }

    void Compiler::visit(tree::Literal * literal) {
        switch (literal->token.type) {
            case parser::TokenType::Null: {
                emit(bytecode::OpCode::NullConst);
                last_type = NullType::get();
            } break;
            case parser::TokenType::True: {
                emit(bytecode::OpCode::TrueConst);
                last_type = BoolType::get();
            } break;
            case parser::TokenType::False: {
                emit(bytecode::OpCode::FalseConst);
                last_type = BoolType::get();
            } break;
            case parser::TokenType::Int: {
                // TODO: Add conversion exception handling
                long long int_val = std::stoll(literal->token.val);
                emit_int(int_val);
                last_type = IntType::get();
            } break;
            case parser::TokenType::Float: {
                // TODO: Add conversion exception handling
                double float_val = std::stod(literal->token.val);
                emit_float(float_val);
                last_type = FloatType::get();
            } break;
            case parser::TokenType::String: {
                // TODO: Add encodings support
                const auto & string_val = literal->token.val;
                emit_string(string_val);
                last_type = StringType::get();
            } break;
            default: {
                throw DevError("Unexpected type of literal token");
            }
        }
    }

    void Compiler::visit(tree::Identifier * id) {
        emit_id(id);
    }

    void Compiler::visit(tree::Infix * infix) {
        // FIXME: I cannot use infix as GetProperty - Invoke, because VM does not know about signatures

//        last_type = nullptr;
//        infix->left->accept(*this);
//        const auto & lhs_t = last_type;
//
//        last_type = nullptr;
//        infix->right->accept(*this);
//        const auto & rhs_t = last_type;


//        switch (infix->op.type) {
//            case parser::TokenType::Add: {
//                const auto & op_method = class_has_method(lhs_t, "add", get(get_any_t(), {rhs_t}, true));
//                if (!op_method) {
//                    error("Unable to resolve infix operator function (add)", infix->pos);
//                }
//
//                bytecode::OpCode opcode;
//                if (op_method->tag == TypeTag::NativeFunc) {
//                    opcode = bytecode::OpCode::InvokeNF;
//                } else {
//                    opcode = bytecode::OpCode::Invoke;
//                }
//
//                emit(opcode);
//                emit(static_cast<uint64_t>(1));
//
//                // Return type
//                last_type = op_method->return_type;
//            } break;
//            default: {
//                // Note: rewrite if infix functions will be added
//                throw DevError("Invalid infix token operator");
//            }
//        }
    }

    void Compiler::visit(tree::Prefix * expr_stmt) {

    }

    void Compiler::visit(tree::Assign * assign) {
        // TODO: ! Disallow globals (and internal modules) reassignment
        assign->value->accept(*this);
        bytecode::OpCode opcode;
        uint64_t operand;
        try {
            operand = resolve_local(scope, assign->id.get());
            opcode = bytecode::OpCode::StoreLocal;
        } catch (IUndefinedEntity & e) {
            operand = make_string(assign->id->get_name());
            opcode = bytecode::OpCode::StoreGlobal;
        }
        emit(opcode);
        emit(operand);
    }

    void Compiler::visit(tree::SetExpr * set_expr) {
        set_expr->left->accept(*this);

        uint64_t name = make_string(set_expr->id->get_name());

        // TODO: Type check
        set_expr->value->accept(*this);

        emit(bytecode::OpCode::SetProperty);
        emit(name);
    }

    void Compiler::visit(tree::GetExpr * get_expr) {
        last_type = nullptr;

        get_expr->left->accept(*this);
        uint64_t name = make_string(get_expr->id->get_name());
        emit(bytecode::OpCode::GetProperty);
        emit(name);
    }

    void Compiler::visit(tree::FuncCall * func_call) {
        last_type = nullptr;
        func_call->left->accept(*this);

        type_ptr expr_type = last_type;
        bytecode::OpCode opcode;
        switch (expr_type->tag) {
            case TypeTag::Func: {
                opcode = bytecode::OpCode::Invoke;
            } break;
            case TypeTag::NativeFunc: {
                opcode = bytecode::OpCode::InvokeNF;
            } break;
//            case TypeTag::Class: {
//    //            opcode = bytecode::OpCode::Construct;
//            } break;
            default: {
                error("Is not a function", func_call->left->pos);
            }
        }

        // We know that expr_type is FuncType
        std::shared_ptr<FuncType> func_type = std::static_pointer_cast<FuncType>(expr_type);
        func_param_t_list arg_types;

        uint64_t arg_count = 0;
        for (const auto & arg : func_call->args) {
            last_type = nullptr;
            arg.val->accept(*this);
            arg_count++;
            // TODO!: Think about `false` stub for default value
            arg_types.push_back(std::make_shared<FuncParamType>(last_type, false));
        }

        if (!func_type->compare(arg_types)) {
            // TODO: Position of first parentheses
            error("Function invocation does not match any declaration", func_call->left->pos);
        }

        emit(opcode);
        emit(arg_count);

        // Return type
        last_type = func_type->return_type;
    }

    void Compiler::visit(tree::MethodCall * method_call) {
        // TODO: Unite FuncCall and MethodCall to one common function

        last_type = nullptr;
        method_call->left->accept(*this);

        const auto & object = last_type;

        // We know that expr_type is FuncType
        func_param_t_list arg_types;
        uint64_t arg_count = 0;
        for (const auto & arg : method_call->args) {
            last_type = nullptr;
            arg.val->accept(*this);
            arg_count++;
            // TODO!: Think about `false` stub for default value
            arg_types.push_back(std::make_shared<FuncParamType>(last_type, false));
        }

        func_t_ptr method_signature = FuncType::get(Any::get(), arg_types);
        func_t_ptr method = object->has_method(method_call->id->get_name(), method_signature, true);

        if (!method) {
            error("Method invocation does not match any declaration in class " + object->name, method_call->left->pos);
        }

        bytecode::OpCode opcode;
        switch (method->tag) {
            case TypeTag::Func: {
                opcode = bytecode::OpCode::InvokeMethod;
            } break;
            case TypeTag::NativeFunc: {
                opcode = bytecode::OpCode::InvokeNFMethod;
            } break;
//            case TypeTag::Class: {
//                //            opcode = bytecode::OpCode::Construct;
//            } break;
            default: {
                error("Is not a method", method_call->left->pos);
            }
        }

        emit(opcode);
        emit(arg_count);

        // Note!: Here's used `method` signature, not `method_signature`,
        //  because we need the signature that we found, not what we requested (type inheritance...)
        emit(make_string(method->mangle_name(method_call->id->get_name())));

        // Return type
        last_type = method->return_type;
    }

    void Compiler::visit(tree::IfExpr * if_expr) {
        if_expr->cond->accept(*this);

        int then_jump = emit_jump(bytecode::OpCode::JumpFalse);
        emit(bytecode::OpCode::Pop);
        if_expr->if_branch->accept(*this);

        int else_jump = emit_jump(bytecode::OpCode::Jump);
        patch_jump(then_jump);
        emit(bytecode::OpCode::Pop);

        if (if_expr->else_branch) {
            if_expr->else_branch->accept(*this);
        }

        patch_jump(else_jump);
    }

    void Compiler::visit(tree::ListExpr * list_expr) {

    }

    void Compiler::visit(tree::GetItem * expr_stmt) {

    }

    void Compiler::visit(tree::SetItem * expr_stmt) {

    }

    void Compiler::visit(tree::DictExpr * expr_stmt) {

    }

    void Compiler::visit(tree::SpreadExpr * spread_expr) {

    }

    /////////////////
    // tree::Types //
    /////////////////
    void Compiler::visit(tree::IdType * id_type) {

    }

    void Compiler::visit(tree::ListType * list_type) {

    }

    void Compiler::visit(tree::DictType * dict_type) {

    }

    void Compiler::visit(tree::GenericType * generic_type) {

    }

    void Compiler::visit(tree::UnionType * union_type) {

    }

    //////////////
    // Bytecode //
    //////////////
    void Compiler::emit(uint8_t byte) {
        chunk.code.push_back(byte);
    }

    void Compiler::emit(bytecode::OpCode opcode) {
        emit(static_cast<uint8_t>(opcode));
    }

    void Compiler::emit(const uint8_t * byte_array, int size) {
        // TODO!: Endianness
        for (uint64_t i = 0; i < size; i++) {
            emit(byte_array[i]);
        }
    }

    void Compiler::emit(uint16_t s) {
        emit(reinterpret_cast<uint8_t*>(&s), 2);
    }

    void Compiler::emit(uint32_t i) {
        emit(reinterpret_cast<uint8_t*>(&i), 4);
    }

    void Compiler::emit(uint64_t l) {
        emit(reinterpret_cast<uint8_t*>(&l), 8);
    }

    ///////////////
    // Constants //
    ///////////////
    void Compiler::emit_int(long long int_val) {
        emit(bytecode::OpCode::IntConst);
        const auto & found = int_constants.find(int_val);
        if (found != int_constants.end()) {
            emit(found->second);
            return;
        }
        // Make int
        chunk.constants.push_back(std::make_shared<bytecode::IntConstant>(int_val, IntType::get()));
        int_constants[int_val] = chunk.constants.size() - 1;
        emit(static_cast<uint64_t>(chunk.constants.size() - 1));
    }

    void Compiler::emit_float(double float_val) {
        emit(bytecode::OpCode::FloatConst);
        const auto & found = float_constants.find(float_val);
        if (found != float_constants.end()) {
            emit(found->second);
            return;
        }
        chunk.constants.push_back(std::make_shared<bytecode::FloatConstant>(float_val, FloatType::get()));
        float_constants[float_val] = chunk.constants.size() - 1;
        emit(static_cast<uint64_t>(chunk.constants.size() - 1));
    }

    void Compiler::emit_string(const std::string & string_val) {
        emit(bytecode::OpCode::StringConst);
        emit(make_string(string_val));
    }

    uint64_t Compiler::make_string(const std::string & string_val) {
        const auto & found = string_constants.find(string_val);
        if (found != string_constants.end()) {
            return found->second;
        }
        chunk.constants.push_back(std::make_shared<bytecode::StringConstant>(string_val, StringType::get()));
        string_constants[string_val] = chunk.constants.size() - 1;
        return chunk.constants.size() - 1;
    }

    ///////////
    // Scope //
    ///////////
    void Compiler::enter_scope() {
        scope_depth++;
        scope = std::make_shared<Scope>(scope);
    }

    void Compiler::exit_scope() {
        scope_depth--;
        if (!scope->parent) {
            throw DevError("Attempt to exit global scope");
        }
        scope = scope->parent;
    }

    ///////////////
    // Variables //
    ///////////////
    uint64_t Compiler::resolve_local(const scope_ptr & _scope, tree::Identifier * id) {
        if (_scope->locals.size() == 0) {
            // local size is unsigned, so I cannot subtract it
            undefined_entity();
        }

        // Note: I need to use (uint64_t)(0 - 1), 'cause unsigned 0 - 1 is not -1
        for (uint64_t i = _scope->locals.size() - 1; i != (uint64_t)(0 - 1); i--) {
            const auto & local = _scope->locals[i];
            if (local.name == id->get_name()) {
                if (!local.is_defined) {
                    log.debug("In 'resolve_local()':");
                    error(id->get_name() + " is not defined", id->pos);
                }
                return i;
            }
        }

        undefined_entity();
    }

    uint64_t Compiler::resolve_upvalue(const scope_ptr & _scope, tree::Identifier * id) {
        if (!_scope->parent) {
            undefined_entity();
        }

        uint64_t local = resolve_local(_scope, id);
        if (local != -1) {
            _scope->parent->locals[local].is_captured = true;
    //        return add_upvalue(_scope, static_cast<uint64_t>(local), true);
        }

        uint64_t upvalue = resolve_upvalue(_scope->parent, id);
        if (upvalue != -1) {
    //        return add_upvalue(_scope, static_cast<uint64_t>(upvalue), false);
        }

        undefined_entity();
    }

    void Compiler::emit_id(tree::Identifier * id) {
        bytecode::OpCode opcode;
        uint64_t operand;

        try {
            operand = resolve_local(scope, id);
            opcode = bytecode::OpCode::LoadLocal;
            last_type = scope->locals.at(operand).type;
        } catch (IUndefinedEntity & e) {
            operand = make_string(id->get_name());
            opcode = bytecode::OpCode::LoadGlobal;

            try {
                const auto & global = globals.at(id->get_name());
                if (!global) {
                    log.debug(id->get_name(), " global is null");
                    throw std::out_of_range(id->get_name());
                }
                last_type = global->type;
            } catch (std::out_of_range & e) {
                log.debug("In 'emit_id()' [LoadGlobal]");
                error(id->get_name() + " is not defined", id->pos);
            }
        }

        emit(opcode);
        emit(operand);
    }

    void Compiler::declare_var(tree::VarDeclKind kind, type_ptr type, tree::Identifier * id) {
        if (scope_depth == 0) {
            return;
        }

        for (const auto & local : scope->locals) {
            if (local.is_defined && local.depth < scope_depth) {
                break;
            }
            if (id->get_name() == local.name) {
                error(id->get_name() + " has been already declared in this scope", id->pos);
            }
        }

        add_local(kind, type, id->get_name());
    }

    void Compiler::add_local(tree::VarDeclKind kind, type_ptr type, const std::string & name) {
        if (scope->locals.size() == UINT32_MAX) {
            throw DevError("Unable to handle too many locals");
        }

        scope->locals.emplace_back(kind, type, name);
    }

    ///////////
    // Jumps //
    ///////////
    int64_t Compiler::emit_jump(bytecode::OpCode jump_instr) {
        emit(jump_instr);
        for (int i = 0; i < bytecode::jump_space; i++) {
            emit(bytecode::U255);
        }
        return chunk.code.size() - bytecode::jump_space;
    }

    void Compiler::patch_jump(int64_t offset) {
        int64_t jump = chunk.code.size() - offset - bytecode::jump_space;

        if (jump > UINT64_MAX) {
            throw DevError("Unable to handle too large jump: " + std::to_string(jump));
        }

        for (int i = bytecode::jump_space - 1; i >= 0; i--) {
            chunk.code[offset + bytecode::jump_space - i - 1] = (jump >> (i * bytecode::U8)) & bytecode::U255;
        }
    }

    ///////////
    // Types //
    ///////////
    type_ptr Compiler::resolve_type(tree::Identifier * id) {
        scope_ptr _scope = scope;

        while (_scope) {
            for (const auto & local : _scope->locals) {
                if (local.name == id->get_name()) {
                    return local.type;
                }
            }
            _scope = _scope->parent;
        }

        const auto & global = globals.find(id->get_name());
        if (global != globals.end()) {
            return global->second->type;
        }

        return nullptr;
    }

    ////////////
    // Errors //
    ////////////
    void Compiler::error(const std::string & msg, const Position & pos) {
        std::string message = msg;
        message += " at " + std::to_string(pos.line) + ":" + std::to_string(pos.column);
        throw CTException(message);
    }

    void Compiler::undefined_entity() {
        throw IUndefinedEntity();
    }
}
