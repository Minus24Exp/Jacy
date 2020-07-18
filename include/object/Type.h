#ifndef TYPE_H
#define TYPE_H

#include "object/Object.h"

class Type;
using type_ptr = std::shared_ptr<Type>;

const auto cast_to_type = [](obj_ptr obj) -> type_ptr { return std::dynamic_pointer_cast<Type>(obj); };

class Type : public Object {
public:
    Type();
    virtual ~Type() = default;

    virtual std::string repr() const override;
};

class ClassType : public Type {
public:
    ClassType(const std::string & class_name);
    virtual ~ClassType() = default;

    std::string repr() const override;

private:
    std::string class_name;
};

#endif