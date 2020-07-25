#ifndef IMPORT_H
#define IMPORT_H

struct Import : Stmt {
    std::string path;

    Import(const Position & pos, const std::string & path) : Stmt(pos, StmtType::Import), path(path) {}
    virtual ~Import() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif