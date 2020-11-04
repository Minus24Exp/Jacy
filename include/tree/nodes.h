// Statements //
#include "tree/Stmt/Block.h"
#include "tree/Stmt/VarDecl.h"
#include "tree/Stmt/FuncDecl.h"
#include "tree/Stmt/ReturnStmt.h"
#include "tree/Stmt/WhileStmt.h"
#include "tree/Stmt/ForStmt.h"
#include "tree/Stmt/ClassDecl.h"
#include "tree/Stmt/Import.h"
#include "tree/Stmt/TypeDecl.h"

// Expressions //
#include "tree/Expr/Literal.h"
#include "tree/Expr/Identifier.h"
#include "tree/Expr/Infix.h"
#include "tree/Expr/Prefix.h"
#include "tree/Expr/Assign.h"
#include "tree/Expr/SetExpr.h"
#include "tree/Expr/GetExpr.h"
#include "tree/Expr/FuncCall.h"
#include "tree/Expr/IfExpr.h"
#include "tree/Expr/ListExpr.h"
#include "tree/Expr/GetItem.h"
#include "tree/Expr/SetItem.h"
#include "tree/Expr/DictExpr.h"

#include "tree/Node.h"
/**
 * This file just includes all nodes
 * But (!important!), include Node.h last
 */