#include "object/List.h"
#include "object/NativeFunc.h"
#include "object/Int.h"
#include "object/String.h"

#include <iostream>

List::List() : Object(ObjectType::List, cList)
{
    define_builtin("__getitem", make_nf(nullptr, "__getitem", { {"index"} }, [this](NFArgs && args){
        int_ptr index_obj = cast_to_i(args["index"]);

        if(!index_obj){
            throw YoctoException("Invalid type of index (int expected)");
        }

        yo_int index = norm_index(index_obj->get_value());

        if(index < 0 || index >= size()){
            throw YoctoException("Index out of list bounds");
        }

        std::cout << "get by index: " << index << std::endl;

        return elements[index];
    }));

    define_builtin("__setitem", make_nf(nullptr, "__setitem", { {"index"}, {"value"} }, [this](NFArgs && args){
        int_ptr index_obj = cast_to_i(args["index"]);
        
        if(!index_obj){
            throw YoctoException("Invalid type of index");
        }

        yo_int index = norm_index(index_obj->get_value());
        if(index >= size()){
            throw YoctoException("Index out of list bounds");
        }

        return elements[index];
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        std::string str = "[";

        for(auto it = elements.begin(); it != elements.end(); it++){
            str += obj_to_str(*it);

            if(it != std::prev(elements.end())){
                str += ", ";
            }
        }
        str += "]";

        return std::make_shared<String>(str);
    }));
}

bool List::truthy() const {
    return size() == 0;
}

std::string List::repr() const {
    return "<List>";
}

ObjList List::get_elements() const {
    return elements;
}

void List::set_elements(const ObjList & elements){
    this->elements = elements;
}

yo_int List::size() const {
    return elements.size();
}

yo_int List::norm_index(yo_int index) const {
    if(index < 0){
        return size() + index;
    }else{
        return index;
    }
}

obj_ptr List::get_item(yo_int index) const {
    if(index >= size()){
        return nullptr;
    }
    return elements.at(index);
}

bool List::set_item(yo_int index, obj_ptr el){
    if(index >= size()){
        return false;
    }
    elements[index] = el;
    return true;
}

void List::append(obj_ptr el){
    elements.push_back(el);
}