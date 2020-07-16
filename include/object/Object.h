#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>
#include "backend/Scope.h"

class String;
class Object;
class Class;

using obj_ptr = std::shared_ptr<Object>;
using ObjList = std::vector<obj_ptr>;

class Object : public Scope, public std::enable_shared_from_this<Object> {
public:
    Object();
    Object(scope_ptr scope, Class * _class);
    virtual ~Object() = default;

    // All objects except Null and Boolean with value `false` are truthy
    virtual bool truthy() const;

    // Represent object as string
    virtual std::string repr() const;

    // Scope //
    obj_ptr get(const std::string & name) const override;

private:
    Class * _class;
};

std::string obj_to_str(obj_ptr obj);

#endif