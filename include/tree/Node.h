#ifndef NODE_H
#define NODE_H

#include "parser/Token.h"
#include "tree/BaseVisitor.h"

#include <memory>
#include <utility>

using jc::parser::Position;

namespace jc::tree {
    struct Node {
        explicit Node(const Position & pos) : pos(pos) {}
        virtual ~Node() = default;

        virtual void accept(BaseVisitor & visitor) = 0;

        Position pos;
    };
}

#endif