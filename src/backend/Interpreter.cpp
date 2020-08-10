#include "backend/Interpreter.h"
#include "Yocto.h"

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

std::string Interpreter::path_dir(const std::string & path){
    return path.substr(0, path.find_last_of("/\\") + 1);
}

std::string Interpreter::resolve_path(const std::string & path){
    std::string resolved = path;

    if(path[0] == '/'){
        return path;
    }

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    if(path[0] == '\\'){
        return path;
    }
    if(path[1] == ':'){
        return path;
    }
    #endif

    if(path.substr(path.find_last_of('.') + 1) != "yo"){
        resolved += ".yo";
    }

    return (dir_stack.empty() ? "" : dir_stack.top()) + resolved;
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

void Interpreter::visit(ForStmt * for_stmt){
    obj_ptr in_obj = eval(for_stmt->In.get());

    if(!in_obj->has_method("__iter")){
        runtime_error(obj_to_str(in_obj) +" is not iterable", for_stmt->In.get());
    }

    func_ptr iter_func = s_cast_to_func(in_obj->get("__iter"));
    obj_ptr iterator = iter_func->bind(in_obj)->call();

    // Iterator must have `next` and `hasNext` methods
    if(!iterator->has_method("next") || !iterator->has_method("hasNext")){
        runtime_error(obj_to_str(in_obj) +" has invalid __iter method (must return iterator object with next and hasNext methods)", for_stmt->In.get());
    }

    func_ptr has_next = s_cast_to_func(iterator->get("hasNext"))->bind(iterator);
    func_ptr next = s_cast_to_func(iterator->get("next"))->bind(iterator);

    std::string for_obj_name = for_stmt->For->get_name();

    enter_scope();

    // `next` called at start
    scope->define(for_obj_name, {LocalDeclType::Var, next->call()});

    while(has_next->call()){
        execute_block(for_stmt->body.get());
        scope->assign(for_obj_name, next->call());
    }

    exit_scope();
}

// ClassDecl //
void Interpreter::visit(ClassDecl * class_decl){
    class_ptr super = nullptr;
    std::string class_name = class_decl->id->get_name();

    if(class_decl->super){
        obj_ptr super_expr = eval(class_decl->super.get());
        if(super_expr->get_obj_type() != ObjectType::Class){
            runtime_error("Invalid expression used as class super (must be type of Class)", class_decl->super.get());
        }
        super = s_cast_to_class(super_expr);
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
    // Set last module name to imported module to track errors
    // and save previous name to return back
    std::string previous_module_name = last_module_name;
    set_last_module_name(import->path);

    std::string path = resolve_path(import->path);

    // Enter module scope
    enter_scope();

    // Push module directory to directory stack
    dir_stack.push(path_dir(path));

    try {
        Yocto::get_instance().run_script(path);
    }catch(FileNotFoundException & e){
        runtime_error("File " + import->path +" not found", import);
    }

    module_ptr module = std::make_shared<Module>(scope->get_locals(), path);

    // Return back to previous module name
    set_last_module_name(previous_module_name);
    
    // Exit module scope
    exit_scope();

    dir_stack.pop();

    // Note: if `entities` is empty then where's nothing to import
    // it just runs source of module
    
    if(!import->entities.empty()){
        for(const auto & entity : import->entities){
            bool defined = false;

            if(entity.all){
                defined = scope->define(entity.as->get_name(), {LocalDeclType::Val, module});
            }else{
                obj_ptr module_object = module->get(entity.object->get_name());
                if(!module_object){
                    runtime_error(entity.object->get_name() +" is not defined in "+ import->path, import);
                }
                defined = scope->define(entity.as->get_name(), {LocalDeclType::Val, module_object});
            }

            if(!defined){
                runtime_error(entity.as->get_name() +" is already defined", import);
            }
        }
    }
}

/////////////////
// Expressions //
/////////////////
void Interpreter::visit(Literal * literal){
    switch(literal->token.type){
        case TokenType::Null:{
            value = null_obj;
        } break;
        case TokenType::Bool:{
            value = make_bool(literal->token.Bool());
        } break;
        case TokenType::Int:{
            value = make_int(literal->token.Int());
        } break;
        case TokenType::Float:{
            value = make_float(literal->token.Float());
        } break;
        case TokenType::String:{
            value = make_string(literal->token.String());
        } break;
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
        } break;
        case ArgsCmpResult::TooMany:{
            runtime_error("Too many arguments in function call "
                          " (maximum "+ max_argc_s +" expected, "
                          + given_argc_s +" given)", func_call);
        } break;
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

    // Negate is used for built-in functions that have one name for truth check
    bool negate = false;

    // Is right associative
    bool r_assoc = false;

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
            value = make_bool(lhs == rhs);
            return;
        } break;
        case Operator::RefNotEq:{
            value = make_bool(lhs != rhs);
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

            value = make_bool(lhs->is(rhs_class));
            return;
        } break;
        case Operator::NotIs:{
            class_ptr rhs_class = cast_to_class(rhs);

            if(!rhs_class){
                runtime_error("Invalid right-hand side in `!is` operator", infix);
            }

            value = make_bool(!lhs->is(rhs_class));
            return;
        } break;
        case Operator::In:{
            magic_func_name = "__contains";
            r_assoc = true;
        } break;
        case Operator::NotIn:{
            magic_func_name = "__contains";
            negate = true;
            r_assoc = true;
        } break;
        case Operator::Pipe:{
            if(rhs->get_obj_type() != ObjectType::Func){
                runtime_error("Invalid right-hand side in pipe operator, function expected", infix->right.get());
            }
            const auto rhs_func = std::static_pointer_cast<BaseFunc>(rhs);
            value = rhs_func->call({lhs});
            return;
        } break;
        default:{
            throw DevError("Unsupported infix operator: `"+ op_name +"`");
        }
    }

    if(r_assoc){
        lhs.swap(rhs);
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

        if(negate){
            bool_ptr b = cast_to_b(value);
            if(!b){
                throw DevError("Negative function returns non-bool value");
            }
            value = b;
        }
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
        } break;
        case Operator::Sub:{
            magic_func_name = "__neg";
        } break;
        default:{
            throw DevError("Unsupported prefix operator: "+ op_name);
        }
    }

    if(!rhs->has(magic_func_name)){
        runtime_error("Invalid right-hand side in prefix " + op_name, prefix->right.get());
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

// Assign //
void Interpreter::visit(Assign * assign){
    // Note: right-associative
    obj_ptr rhs = eval(assign->value.get());

    std::string name = assign->id->get_name();

    std::string augment_func_name;
    switch(assign->assign_op){
        case Operator::Assign: break;

        case Operator::AddAssign:{
            augment_func_name = "__add";
        } break;

        default:{
            throw DevError("Attempt to use non-assign operator as augmented");
        }
    }

    if(!scope->has(name)){
        runtime_error(name +" is not defined", assign->id.get());
    }

    obj_ptr augment_value = nullptr;
    if(!augment_func_name.empty()){
        obj_ptr local = scope->get(name);

        if(!rhs->has(augment_func_name)){
            runtime_error("Invalid left-hand side in augmented assignment", assign->id.get());
        }

        func_ptr magic_func = std::dynamic_pointer_cast<BaseFunc>(rhs->get(augment_func_name));

        if(!magic_func){
            runtime_error(augment_func_name +" must be a function", assign->id.get());
        }

        try{
            augment_value = magic_func->call({ rhs });
        }catch(RuntimeException & e){
            throw e;
        }catch(YoctoException & e){
            runtime_error(e.what(), assign);
        }
    }

    int result = scope->assign(name, augment_value ? augment_value : rhs);

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
        runtime_error(obj_to_str(lhs) +" does not have member "+ name, set_expr);
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

    obj_ptr field = lhs->get(name);

    if(field == nullptr){
        runtime_error(name +"is not defined", get_expr);
    }
    
    if(field->get_obj_type() == ObjectType::Func){
        value = s_cast_to_func(field)->bind(lhs);
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

void Interpreter::visit(ListExpr * list_expr){
    list_ptr list = std::make_shared<List>();
    ObjList elements;

    for(const auto & expr : list_expr->elements){
        obj_ptr el = eval(expr.get());
        elements.push_back(el);
    }

    list->set_elements(elements);

    value = list;
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

void Interpreter::visit(DictExpr * dict_expr){
    dict_ptr dict = std::make_shared<Dict>();

    for(const auto & el : dict_expr->elements){
        obj_ptr key = eval(el.key.get());
        obj_ptr val = eval(el.val.get());

        try{
            dict->set_item(key, val);
        }catch(YoctoException & e){
            runtime_error(e.what(), el.key.get());
        }
    }

    value = dict;
}

////////////
// Errors //
////////////
void Interpreter::runtime_error(const std::string & msg, const Position & pos){
    throw RuntimeException(msg, pos, last_module_name);
}

void Interpreter::runtime_error(const std::string & msg, Node * n){
    runtime_error(msg, n->pos);
}