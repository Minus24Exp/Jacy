#ifndef FUNCDECL_H
#define FUNCDECL_H

#include "tree/Identifier.h"
#include "tree/Block.h"

struct FuncParam {
	id_ptr id;
};

using ParamList = std::vector<FuncParam>;

struct FuncDecl : Stmt {
	id_ptr id;
	ParamList params;
	block_ptr body;

	FuncDecl(id_ptr id, const ParamList & params, block_ptr body)
		: Stmt(StmtType::FuncDecl), id(id), params(params), body(body) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif