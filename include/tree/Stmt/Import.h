#ifndef IMPORT_H
#define IMPORT_H

#include "tree/Stmt/Stmt.h"

namespace jc::tree {
    struct ImportEntity {
        // Note: Can be `all=true` or `object!=nullptr`
        bool all;
        id_ptr object;

        id_ptr as;

        ImportEntity(bool all, id_ptr object, id_ptr as) : all(all), object(object), as(as) {}
        virtual ~ImportEntity() = default;
    };

    using ImportEntityList = std::vector<ImportEntity>;

    struct Import : Stmt {
        ImportEntityList entities;
        std::string path;

        Import(const Position & pos, const std::string & path, const ImportEntityList & entities)
                : Stmt(pos, StmtType::Import), path(path), entities(entities) {}
        Import(const Position & pos, const std::string & path)
                : Stmt(pos, StmtType::Import), path(path) {}
        virtual ~Import() = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif