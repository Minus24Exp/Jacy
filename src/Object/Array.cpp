#include "object/Array.h"
#include "object/Int.h"
#include "object/String.h"

Array::Array(){
    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        std::string str;

        str += "[";
        // TODO: Rewrite for varargs in future
        yo_int N = size();
        for(yo_int i = 0; i < N; i++){
            str += obj_to_str(elements[i])->get_value();
            if(i < N - 1){
                str += ", ";
            }
        }
        str += "]";

        return std::make_shared<String>(str);
    }));

    define_nf("size", make_nf(nullptr, "size", {}, [this](NFArgs && args){
        return std::make_shared<Int>(size());
    }));

    define_nf("__getitem", make_nf(nullptr, "__getitem", { {"index"} }, [this](NFArgs && args){
        int_ptr index_obj = cast_to_i(args["index"]);
       
        if(!index_obj){
            throw YoctoException("Invalid type of index");
        }

        yo_int index = abs_index(index_obj->get_value());

        if(index >= size()){
            throw YoctoException("Index out of array bounds");
        }

        return elements[index];
    }));

    define_nf("__setitem", make_nf(nullptr, "__setitem", { {"index"}, {"value"} }, [this](NFArgs && args){
        int_ptr index_obj = cast_to_i(args["index"]);
        
        if(!index_obj){
            throw YoctoException("Invalid type of index");
        }

        yo_int index = abs_index(index_obj->get_value());
        if(index >= size()){
            throw YoctoException("Index out of array bounds");
        }

        return elements[index];
    }));
}

bool Array::truthy() const {
    return size() != 0;
}

std::string Array::repr() const {
    return "<Array:>";
}

ObjList Array::get_elements() const {
    return elements;
}

void Array::set_elements(const ObjList & elements){
    this->elements = elements;
}

yo_int Array::size() const {
    return elements.size();
}

yo_int Array::abs_index(yo_int index) const {
    if(index < 0){
        // Note: We, of course, need to sum size with negative index
        return size() + index;
    }else{
        return index;
    }
}

obj_ptr Array::get_item(yo_int index) const {
    // return nullptr if index of out bounds
    // Note: nullptr does not equal to null_obj,
    // so arrays can contain Null elements
    if(index >= size()){
        return nullptr;
    }
    return elements.at(index);
}

bool Array::set_item(yo_int index, obj_ptr el){
    if(index >= size()){
        return false;
    }
    elements[index] = el;
    return true;
}

void Array::append(obj_ptr el){
    elements.push_back(el);
}