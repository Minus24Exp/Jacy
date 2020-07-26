#ifndef LIST_H
#define LIST_H

#include "object/List.h"

class List;
using list_ptr = std::shared_ptr<List>;

class List {
public:
    List();
    virtual ~List() = default;

private:
    bool truthy() const override;
    std::string repr() const override;
};

#endif