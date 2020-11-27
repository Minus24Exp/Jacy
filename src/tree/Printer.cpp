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

        visit(var_decl->id.get());

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
            func_decl->params[i].id->accept(*this);
            if (i < func_decl->params.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ") ";
        func_decl->body->accept(*this);
    }

    void Printer::visit(WhileStmt * w) {
        print_indent();
        std::cout << "while ";
        w->cond->accept(*this);
        std::cout << " ";
        w->body->accept(*this);
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
        type_decl->type_expr->accept(*this);
    }

    /////////////////
    // Expressions //
    /////////////////
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
            func_call->args[i].val->accept(*this);
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
        std::cout << "{\n";
        for (const auto & it : dict->elements) {
            if (it.id_key) {
                std::cout << it.id_key->get_name();
            } else {
                std::cout << "[";
                it.expr_key->accept(*this);
                std::cout << "]";
            }
            std::cout << ": ";
            it.val->accept(*this);
            std::cout << ",\n";
        }
        std::cout << "\n}";
    }

    void Printer::visit(MethodCall * method_call) {
        method_call->left->accept(*this);
        std::cout << ".";
        method_call->id->accept(*this);
        std::cout << "(";
        for (size_t i = 0; i < method_call->args.size(); i++) {
            method_call->args.at(i)->accept(*this);
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
}
