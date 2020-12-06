#include "tree/Printer.h"

namespace jc::tree {
    Printer::Printer() = default;

    void Printer::print_indent() const {
        // Note: Use print indent only before statement printing not before expression
        std::string str(indent * 4, ' ');
        std::cout << str;
    }

    void Printer::print(const StmtList & tree) {
        indent = 0;
        for (int i = 0; i < tree.size(); i++) {
            tree[i]->accept(*this);
            if (i < tree.size() - 1) {
                std::cout << std::endl;
            }
        }
    }

    ////////////////
    // Statements //
    ////////////////
    void Printer::visit(ExprStmt * expr_stmt) {
        print_indent();
        expr_stmt->expr->accept(*this);
    }

    void Printer::visit(Block * block) {
        std::cout << "{\n";
        indent++;
        for (int i = 0; i < block->stmts.size(); i++) {
            block->stmts[i]->accept(*this);
            if (i < block->stmts.size() - 1) {
                std::cout << std::endl;
            }
        }
        indent--;
        std::cout << std::endl;
        print_indent();
        std::cout << "}";
    }

    void Printer::visit(VarDecl * var_decl) {
        print_indent();
        if (var_decl->kind == VarDeclKind::Var) {
            std::cout << "var";
        } else if (var_decl->kind == VarDeclKind::Val) {
            std::cout << "val";
        }
        std::cout << " ";

        var_decl->id->accept(*this);

        if (var_decl->type) {
            std::cout << ": ";
            var_decl->type->accept(*this);
        }

        if (var_decl->assign_expr) {
            std::cout << " = ";
            var_decl->assign_expr->accept(*this);
        }
    }

    void Printer::visit(FuncDecl * func_decl) {
        print_indent();
        std::cout << "func ";
        func_decl->id->accept(*this);
        std::cout << "(";
        for (int i = 0; i < func_decl->params.size(); i++) {
            if (func_decl->params.at(i).vararg) {
                std::cout << "...";
            }
            func_decl->params.at(i).id->accept(*this);
            if (func_decl->params.at(i).type) {
                std::cout << ": ";
                func_decl->params.at(i).type->accept(*this);
            }
            if (i < func_decl->params.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")";

        if (func_decl->return_type) {
            std::cout << ": ";
            func_decl->return_type->accept(*this);
        }

        std::cout << " ";

        func_decl->body->accept(*this);
    }

    void Printer::visit(WhileStmt * while_stmt) {
        print_indent();
        std::cout << "while ";
        while_stmt->cond->accept(*this);
        std::cout << " ";
        while_stmt->body->accept(*this);
    }

    void Printer::visit(ForStmt * for_stmt) {
        print_indent();
        std::cout << "for ";
        for_stmt->For->accept(*this);
        std::cout << " in ";
        for_stmt->In->accept(*this);
        std::cout << " ";
        for_stmt->body->accept(*this);
    }

    void Printer::visit(ReturnStmt * return_stmt) {
        print_indent();
        std::cout << "return ";
        return_stmt->accept(*this);
    }

    void Printer::visit(ClassDecl * class_decl) {
        print_indent();
        std::cout << "class ";
        class_decl->id->accept(*this);

        if (class_decl->super) {
            std::cout << " : ";
            class_decl->super->accept(*this);
        }

        std::cout << " {\n";

        indent++;
        for (const auto & field : class_decl->fields) {
            field->accept(*this);
            std::cout << std::endl;
        }
        indent--;

        print_indent();
        std::cout << "}";
    }

    void Printer::visit(Import * import) {
        std::cout << "import ";

        const auto N = import->entities.size();
        for (int i = 0; i < N; i++) {
            const auto entity = import->entities[i];

            if (entity.all) {
                std::cout << "*";
            } else {
                entity.object->accept(*this);
            }

            if (entity.as) {
                std::cout << " as ";
                entity.as->accept(*this);
            }

            if (i < N - 1) {
                std::cout << ", ";
            }
        }

        if (N > 0) {
            std::cout << " from ";
        }

        std::cout << "\"" << import->path << "\"";
    }

    void Printer::visit(TypeDecl * type_decl) {
        print_indent();
        std::cout << "type ";
        type_decl->id->accept(*this);
        std::cout << " = ";
        type_decl->type->accept(*this);
    }

    /////////////////
    // Expressions //
    /////////////////
    void Printer::visit(Grouping * grouping) {
        std::cout << "(";
        grouping->expr->accept(*this);
        std::cout << ")";
    }

    void Printer::visit(Literal * literal) {

        std::cout << literal->token.val;
    }

    void Printer::visit(Identifier * id) {
        std::cout << id->token.val;
    }

    void Printer::visit(Infix * infix) {
        infix->left->accept(*this);
        std::cout << " " << op_to_str(infix->op.type) << " ";
        infix->right->accept(*this);
    }

    void Printer::visit(Prefix * prefix) {
        std::cout << op_to_str(prefix->op.type);
        prefix->right->accept(*this);
    }

    void Printer::visit(Assign * assign) {
        assign->id->accept(*this);
        std::cout << " ";
        std::cout << op_to_str(assign->assign_op.type);
        std::cout << " ";
        assign->value->accept(*this);
    }

    void Printer::visit(SetExpr * set_expr) {
        set_expr->left->accept(*this);
        std::cout << ".";
        set_expr->id->accept(*this);
        std::cout << " = ";
        set_expr->value->accept(*this);
    }

    void Printer::visit(GetExpr * get_expr) {
        get_expr->left->accept(*this);
        std::cout << ".";
        get_expr->id->accept(*this);
    }

    void Printer::visit(FuncCall * func_call) {
        func_call->left->accept(*this);

        std::cout << "(";
        for (int i = 0; i < func_call->args.size(); i++) {
            if (func_call->args.at(i).spread) {
                std::cout << "...";
            }
            func_call->args.at(i).val->accept(*this);
            if (i < func_call->args.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")";
    }

    void Printer::visit(IfExpr * if_expr) {
        std::cout << "if ";
        if_expr->cond->accept(*this);
        std::cout << " ";
        if_expr->if_branch->accept(*this);

        if (if_expr->else_branch) {
            std::cout << " else ";
            if_expr->else_branch->accept(*this);
        }
    }

    void Printer::visit(ListExpr * list) {
        std::cout << "[";
        for (size_t i = 0; i < list->elements.size(); i++) {
            list->elements[i]->accept(*this);
            if (i < list->elements.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }

    void Printer::visit(GetItem * get_item) {
        get_item->left->accept(*this);
        std::cout << "[";
        get_item->index->accept(*this);
        std::cout << "]";
    }

    void Printer::visit(SetItem * set_item) {
        set_item->left->accept(*this);
        std::cout << "[";
        set_item->index->accept(*this);
        std::cout << "]";
        std::cout << " = ";
        set_item->value->accept(*this);
    }

    void Printer::visit(DictExpr * dict) {
        if (dict->elements.empty()) {
            std::cout << "{}";
            return;
        }

        if (dict->elements.size() == 1) {
            const auto & el = dict->elements.at(0);
            std::cout << "{";
            if (dict->elements.at(0).id_key) {
                std::cout << el.id_key->get_name();
            } else {
                std::cout << "[";
                el.expr_key->accept(*this);
                std::cout << "]";
            }
            std::cout << ": ";
            el.val->accept(*this);
            std::cout << "}";
            return;
        }

        std::cout << "{\n";
        for (size_t i = 0; i < dict->elements.size(); i++) {
            const auto & el = dict->elements.at(i);
            if (el.id_key) {
                std::cout << el.id_key->get_name();
            } else {
                std::cout << "[";
                el.expr_key->accept(*this);
                std::cout << "]";
            }
            std::cout << ": ";
            el.val->accept(*this);

            if (i < dict->elements.size() - 1) {
                std::cout << ",\n";
            }
        }
        std::cout << "\n}";
    }

    void Printer::visit(MethodCall * method_call) {
        method_call->left->accept(*this);
        std::cout << ".";
        method_call->id->accept(*this);
        std::cout << "(";
        for (size_t i = 0; i < method_call->args.size(); i++) {
            if (method_call->args.at(i).spread) {
                std::cout << "...";
            }
            method_call->args.at(i).val->accept(*this);
            if (i < method_call->args.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")";
    }

    void Printer::visit(SpreadExpr * spread_expr) {
        std::cout << "...";
        spread_expr->expr->accept(*this);
    }

    ///////////
    // Types //
    ///////////
    void Printer::visit(IdType * id_type) {
        id_type->id->accept(*this);
    }

    void Printer::visit(ListType * list_type) {
        std::cout << "[";
        list_type->type->accept(*this);
        std::cout << "]";
    }

    void Printer::visit(DictType * dict_type) {
        std::cout << "{";
        dict_type->key->accept(*this);
        std::cout << ": ";
        dict_type->val->accept(*this);
        std::cout << "}";
    }

    void Printer::visit(GenericType * generic_type) {
        generic_type->id->accept(*this);
        std::cout << "<";
        for (size_t i = 0; i < generic_type->types.size(); i++) {
            generic_type->types.at(i)->accept(*this);
            if (i < generic_type->types.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ">";
    }

    void Printer::visit(UnionType * union_type) {
        union_type->left->accept(*this);
        std::cout << " | ";
        union_type->right->accept(*this);
    }

    void Printer::visit(FuncType * func_type) {
        std::cout << "(";
        for (size_t i = 0; i < func_type->params_t.size(); i++) {
            func_type->params_t.at(i)->accept(*this);
            if (i < func_type->params_t.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ") -> ";
        func_type->return_type->accept(*this);
    }
}
