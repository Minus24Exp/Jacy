#include "object/Type.h"

Type::Type(){}

std::string Type::repr() const {
    return "<type>";   
}

ClassType::ClassType(const std::string & class_name) : class_name(class_name) {}

std::string ClassType::repr() const {
    
}