#include "backend/Interpreter.h"

Interpreter::Interpreter(){
    value = nullptr;
}

void Interpreter::interpret(const StmtList & tree){
    try{
        for(const auto & stmt : tree){
            execute(stmt.get());
        }
    }catch(ReturnValue & return_value){
        // We need to catch return_value, if return statement
        // was not in function
        runtime_error("Unexpected return statement", return_value.pos);
    }
}

void Interpreter::enter_scope(scope_ptr new_scope){
    if(new_scope){
        scope = new_scope;
    }else{
        scope = std::make_shared<Scope>(scope);
    }
}

void Interpreter::exit_scope(){
    scope = scope->get_parent();
    if(!scope){
        throw YoctoException("Attempt to exit global scope");
    }
}

void Interpreter::execute(Stmt * stmt){
    stmt->accept(*this);
}

obj_ptr Interpreter::eval(Expr * expr){
    expr->accept(*this);
    return std::move(value);
}

void Interpreter::execute_block(Block * block, scope_ptr new_scope){
    enter_scope(new_scope);
    for(const auto & stmt : block->stmts){
        execute(stmt.get());
    }
    exit_scope();
}

////////////////
// Statements //
////////////////
void Interpreter::visit(ExprStmt * expr_stmt){
    value = eval(expr_stmt->expr.get());
}

// Block //
void Interpreter::visit(Block * block){
    execute_block(block);
}

// VarDecl //
void Interpreter::visit(VarDecl * var_decl){
    // Important difference between nullptr and null_obj
    // if variable is defined, but not assigned then
    // it's nullptr, and if it's `val` then it can be assigned
    // somewhere after

    value = nullptr;

    if(var_decl->assign_expr){
        value = eval(var_decl->assign_expr.get());
        std::cout << "VarDecl assign_expr: " << obj_to_str(value) << std::endl;
    }

    // TODO: Think about cloning...
    LocalDeclType decl_type = LocalDeclType::Var;
    if(var_decl->decl == VarDeclType::Val){
        decl_type = LocalDeclType::Val;
    }

    bool defined = scope->define(var_decl->id->get_name(), {decl_type, value});
    if(!defined){
        // If tried to redefine
        runtime_error("Redefinition of variable "+ var_decl->id->get_name(), var_decl);
        return;
    }

    value = nullptr;
}

// FuncDecl //
void Interpreter::visit(FuncDecl * func_decl){
    std::string func_name = func_decl->id->get_name();

    ParamList params;
    for(auto & p : func_decl->params){
        obj_ptr default_val = nullptr; 
        if(p.default_val){
            default_val = eval(p.default_val.get());
        }
        params.push_back({p.id->get_name(), default_val});
    }

    obj_ptr func = std::make_shared<Func>(scope, func_name, params, std::move(func_decl->body));

    bool defined = scope->define(func_name, {LocalDeclType::Val, func});
    if(!defined){
        runtime_error("Redefinition of function "+ func_name, func_decl);
    }
}

// ReturnStmt //
void Interpreter::visit(ReturnStmt * return_stmt){
    if(return_stmt->expr){
        value = eval(return_stmt->expr.get());
    }else{
        value = null_obj;
    }
    throw ReturnValue{value, return_stmt->pos};
}

// WhileStmt //
void Interpreter::visit(WhileStmt * w){
    while(eval(w->cond.get())->truthy()){
        execute_block(w->body.get());
    }
}

// ClassDecl //
void Interpreter::visit(ClassDecl * class_decl){
    class_ptr super = nullptr;
    std::string class_name = class_decl->id->get_name();

    if(class_decl->super_id){
        std::string super_name = class_decl->super_id->get_name();
        super = std::dynamic_pointer_cast<Class>(scope->get(super_name));
        if(!super){
            runtime_error("Unable to use "+ super_name +" as super class", class_decl->super_id.get());
        }
    }

    scope->define(class_name, {LocalDeclType::Val, nullptr});

    class_ptr _class = std::make_shared<Class>(class_name, super);

    // Enter virtual scope to store class fields
    enter_scope();
    
    for(const auto & f : class_decl->fields){
        execute(f.get());
    }
    
    try{
        _class->set_instance_fields(scope->get_locals());
    }catch(RuntimeException & e){
        throw e;
    }catch(YoctoException & e){
        runtime_error(e.what(), class_decl);
    }

    exit_scope();

    scope->assign(class_name, _class);
}

void Interpreter::visit(Import * import){
    std::cout << "visit import" << std::endl;
}

/////////////////
// Expressions //
/////////////////
void Interpreter::visit(Literal * literal){
    switch(literal->token.type){
        case TokenType::Null:{
            value = null_obj;
            break;
        }
        case TokenType::Bool:{
            value.reset(new Bool(literal->token.Bool()));
            break;
        }
        case TokenType::Int:{
            value.reset(new Int(literal->token.Int()));
            break;
        }
        case TokenType::Float:{
            value.reset(new Float(literal->token.Float()));
            break;
        }
        case TokenType::String:{
            value.reset(new String(literal->token.String()));
            break;
        }
    }
}

// Identifier //
void Interpreter::visit(Identifier * id){
    obj_ptr obj = scope->get(id->get_name());
    if(!obj){
        runtime_error(id->get_name() + " is not defined", id);
        return;
    }
    value = obj;
}

// FuncCall //
void Interpreter::visit(FuncCall * func_call){
    obj_ptr lhs = eval(func_call->left.get());

    ObjList args;
    for(const auto & arg : func_call->args){
        args.push_back(eval(arg.get()));
    }

    Callable * callable = dynamic_cast<Callable*>(lhs.get());

    if(!callable){
        runtime_error("Invalid left-hand side in function call", func_call->left.get());
        return;
    }

    // Compare args and check for errors
    const auto required_argc_s = std::to_string(callable->required_argc());
    const auto max_argc_s = std::to_string(callable->argc());
    const auto given_argc_s = std::to_string(args.size());
    switch(callable->cmp_args(args)){
        case ArgsCmpResult::TooFew:{
            runtime_error("Too few arguments in function call"
                          " (at least " + required_argc_s +" expected, "
                          + given_argc_s +" given)", func_call);
            break;
        }
        case ArgsCmpResult::TooMany:{
            runtime_error("Too many arguments in function call "
                          " (maximum "+ max_argc_s +" expected, "
                          + given_argc_s +" given)", func_call);
            break;
        }
    }
    
    static Callable * last_callable = nullptr;

    if(last_callable != callable){
        callable->reset_recursion_depth();
    }

    last_callable = callable;

    try{
        value = callable->call(std::move(args));
    }catch(RuntimeException & e){
        throw e;
    }catch(YoctoException & e){
        runtime_error(e.what(), func_call);
    }

    if(!value){
        value = null_obj;
    }

    last_callable = nullptr;
}

// Infix //
void Interpreter::visit(Infix * infix){
    obj_ptr lhs = eval(infix->left.get());
    obj_ptr rhs = eval(infix->right.get());

    std::string op_name = op_to_str(infix->op.op());
    std::string magic_func_name;

    switch(infix->op.op()){
        case Operator::Add:{
            magic_func_name = "__add";
        } break;
        case Operator::Sub:{
            magic_func_name = "__sub";
        } break;
        case Operator::Mul:{
            magic_func_name = "__mul";
        } break;
        case Operator::Div:{
            magic_func_name = "__div";
        } break;
        case Operator::Mod:{
            magic_func_name = "__mod";
        } break;
        case Operator::Exp:{
            magic_func_name = "__pow";
        } break;
        case Operator::Eq:{
            magic_func_name = "__eq";
        } break;
        case Operator::RefEq:{
            value = std::make_shared<Bool>(lhs == rhs);
            return;
        } break;
        case Operator::RefNotEq:{
            value = std::make_shared<Bool>(lhs != rhs);
            return;
        } break;
        case Operator::Range:{
            magic_func_name = "__range";
        } break;
        case Operator::RangeLE:{
            magic_func_name = "__range_le";
        } break;
        case Operator::RangeRE:{
            magic_func_name = "__range_re";
        } break;
        case Operator::RangeBothE:{
            magic_func_name = "__range_bothe";
        } break;
        case Operator::Is:{
            class_ptr rhs_class = cast_to_class(rhs);

            if(!rhs_class){
                runtime_error("Invalid right-hand side in `is` operator", infix);
            }

            value = std::make_shared<Bool>(lhs->is(rhs_class));
            return;
        } break;
        case Operator::NotIs:{
            class_ptr rhs_class = cast_to_class(rhs);

            if(!rhs_class){
                runtime_error("Invalid right-hand side in `!is` operator", infix);
            }

            value = std::make_shared<Bool>(!lhs->is(rhs_class));
            return;
        } break;
        default:{
            throw DevError("Unsupported infix operator: `"+ op_name +"`");
        }
    }

    obj_ptr magic_func_field;

    if(lhs->has(magic_func_name)){
        magic_func_field = lhs->get(magic_func_name);
    }else{
        runtime_error("Invalid left-hand side in infix "+ op_name, infix);
    }

    func_ptr magic_func = std::dynamic_pointer_cast<BaseFunc>(magic_func_field);

    if(!magic_func){
        runtime_error(magic_func_name +" must be a function", infix);
    }

    try{
        value = magic_func->call({rhs});
    }catch(RuntimeException & e){
        throw e;
    }catch(YoctoException & e){
        runtime_error(e.what(), infix);
    }
}

// Prefix //
void Interpreter::visit(Prefix * prefix){
    obj_ptr rhs = eval(prefix->right.get());

    std::string op_name = op_to_str(prefix->op.op());

    std::string magic_func_name;
    switch(prefix->op.op()){
        case Operator::Not:{
            magic_func_name = "__not";
            break;
        }
        default:{
            throw DevError("Unsupported prefix operator: "+ op_name);
        }
    }

    if(!rhs->has(magic_func_name)){
        runtime_error("Invalid right-hand side in prefix " + op_name, prefix);
    }

    func_ptr magic_func = std::dynamic_pointer_cast<BaseFunc>(rhs->get(magic_func_name));

    if(!magic_func){
        runtime_error(magic_func_name +" must be a function", prefix);
    }

    try{
        value = magic_func->call();
    }catch(RuntimeException & e){
        throw e;
    }catch(YoctoException & e){
        runtime_error(e.what(), prefix);
    }
}

// Postfix //
void Interpreter::visit(Postfix * postfix){
    std::cout << "visit postfix" << std::endl;
}

// Assign //
void Interpreter::visit(Assign * assign){
    // Note: right-associative
    obj_ptr rhs = eval(assign->value.get());

    std::string name = assign->id->get_name();

    int result = scope->assign(name, rhs);

    if(result == 0){
        runtime_error(name +" is not defined", assign);
    }else if(result == -1){
        runtime_error("Unable to reassign `val` "+ name, assign);
    }
}

// SetExpr //
void Interpreter::visit(SetExpr * set_expr){
    // Note: right-associative

    obj_ptr rhs = eval(set_expr->value.get());
    obj_ptr lhs = eval(set_expr->left.get());

    std::string name = set_expr->id->get_name();

    int result = lhs->set(name, rhs);

    if(result == 0){
        runtime_error(lhs->repr() +" does not have member "+ name, set_expr);
    }else if(result == -1){
        runtime_error("Unable to reassign `val` "+ name, set_expr);
    }
}

// GetExpr //
void Interpreter::visit(GetExpr * get_expr){
    // Note: Left-associative
    
    obj_ptr lhs = eval(get_expr->left.get());

    std::string name = get_expr->id->get_name();

    if(!lhs->has(name)){
        runtime_error(lhs->repr() +" does not have member "+ name, get_expr);
    }

    value = lhs->get(name);

    if(value->get_obj_type() == ObjectType::Func){
        value = std::static_pointer_cast<BaseFunc>(value)->bind(scope, lhs);
    }
}

// IfExpr //
void Interpreter::visit(IfExpr * if_expr){
    auto cond = eval(if_expr->cond.get());

    if(cond->truthy()){
        execute_block(if_expr->if_branch.get());
    }else if(if_expr->else_branch){
        execute_block(if_expr->else_branch.get());
    }
}

void Interpreter::visit(ArrayExpr * array){
    array_ptr array_obj = std::make_shared<Array>();
    ObjList elements;

    for(const auto & expr : array->elements){
        obj_ptr el = eval(expr.get());
        elements.push_back(el);
    }

    array_obj->set_elements(elements);

    value = array_obj;
}

void Interpreter::visit(GetItem * get_item){
    obj_ptr index = eval(get_item->index.get());

    obj_ptr lhs = eval(get_item->left.get());

    if(!lhs->has("__getitem")){
        runtime_error("Cannot get item from "+ obj_to_str(lhs), get_item);
    }

    func_ptr magic_func = cast_to_func(lhs->get("__getitem"));

    if(!magic_func){
        runtime_error("__getitem must a function", get_item);
    }

    try{
        value = magic_func->call({ index });
    }catch(RuntimeException & e){
        throw e;
    }catch(YoctoException & e){
        runtime_error(e.what(), get_item);
    }
}

void Interpreter::visit(SetItem * set_item){
    obj_ptr value = eval(set_item->value.get());
    obj_ptr index = eval(set_item->index.get());
    obj_ptr lhs = eval(set_item->left.get());

    if(!lhs->has("__setitem")){
        runtime_error("Cannot set item for "+ obj_to_str(lhs), set_item);
    }

    func_ptr magic_func = cast_to_func(lhs->get("__setitem"));

    if(!magic_func){
        runtime_error("__setitem must a function", set_item);
    }

    try{
        value = magic_func->call({ index, value });
    }catch(RuntimeException & e){
        throw e;
    }catch(YoctoException & e){
        runtime_error(e.what(), set_item);
    }
}

void Interpreter::visit(DictExpr * dict){
    std::cout << "visit DictExpr" << std::endl;
}

////////////
// Errors //
////////////
void Interpreter::runtime_error(const std::string & msg, const Position & pos){
    throw RuntimeException(msg, pos);
}

void Interpreter::runtime_error(const std::string & msg, Node * n){
    runtime_error(msg, n->pos);
}