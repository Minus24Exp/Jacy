#ifndef FUNCDECL_H
#define FUNCDECL_H

#include "tree/Identifier.h"
#include "tree/Block.h"
#include "tree/Node.h"

struct FuncParam {
	Identifier & id;
};

using ParamList = std::vector<FuncParam>;

struct FuncDecl : Statement {
	Identifier & id;
	ParamList params;
	Block & body;

	FuncDecl(Identifier & id, const ParamList & params, Block & body)
		: id(id), params(params), body(body) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif