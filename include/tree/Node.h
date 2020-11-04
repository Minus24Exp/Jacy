#ifndef NODE_H
#define NODE_H

#include <memory>
#include "parser/Token.h"
#include "tree/BaseVisitor.h"

struct Node {
    explicit Node(const Position & pos) : pos(pos) {}
    virtual ~Node() = default;

    virtual void accept(BaseVisitor & visitor) = 0;

    Position pos;
};

#endif