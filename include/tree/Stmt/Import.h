#ifndef IMPORT_H
#define IMPORT_H

struct Import : Stmt {
    std::string path;
    id_ptr as;

    Import(const Position & pos, const std::string & path, id_ptr as)
        : Stmt(pos, StmtType::Import), path(path), as(as) {}
    virtual ~Import() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif