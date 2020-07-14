#ifndef FUNC_H
#define FUNC_H

#include "tree/Stmt/FuncDecl.h"
#include "object/BaseFunc.h"

/**
 * Func - Callable object that works with user-defined functions
 */

class Func : public BaseFunc {
public:
    Func(scope_ptr closure,
         const std::string & name,
         const ParamList & params,
         block_ptr body);
    virtual ~Func() = default;
    
    // BaseFunc //
    obj_ptr call(const ObjList & args) override;

    obj_ptr bind(obj_ptr instance) override;

private:
    block_ptr body;
};

#endif