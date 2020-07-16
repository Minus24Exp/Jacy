#ifndef LOCAL_H
#define LOCAL_H

#include <memory>

class Object;
using obj_ptr = std::shared_ptr<Object>;

class Local;
using LocalMap = std::unordered_map<std::string, Local>;

enum class LocalDeclType {
    Var,
    Val
};

class Local {
public:
    Local(LocalDeclType decl_type, obj_ptr val) : decl_type(decl_type), val(val) {}
    virtual ~Local() = default;

    LocalDeclType decl_type;
    obj_ptr val;
};

#endif