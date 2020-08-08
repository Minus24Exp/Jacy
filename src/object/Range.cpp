// #include "object/Range.h"
// #include "object/Int.h"
// #include "object/Float.h"
// #include "object/String.h"

// ///////////
// // Range //
// ///////////
// Range::Range(RangeExcl excl) : Object(ObjectType::Range, cRange), excl(excl) {}

// //////////////
// // IntRange //
// //////////////
// IntRange::IntRange(std::shared_ptr<Int> start, std::shared_ptr<Int> end, RangeExcl excl)
//     : Range(excl)
// {
//     if(excl == RangeExcl::Left || excl == RangeExcl::Both){
//         first = start->get_value() + 1;
//     }else{
//         first = start->get_value();
//     }

//     if(excl == RangeExcl::Right || excl == RangeExcl::Both){
//         last = end->get_value() - 1;
//     }else{
//         last = end->get_value();
//     }

//     define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
//         std::string str;

//         if(this->start){
//             str += obj_to_str(this->start);
//         }

//         switch(this->excl){
//             case RangeExcl::None:{
//                 str += "...";
//             } break;
//             case RangeExcl::Left:{
//                 str += ">..";
//             } break;
//             case RangeExcl::Right:{
//                 str += "..<";
//             } break;
//             case RangeExcl::Both:{
//                 str += ">.<";
//             } break;
//         }

//         if(this->end){
//             str += obj_to_str(this->end);
//         }

//         return make_string(str);
//     }));

//     define_builtin("__iter", make_nf(nullptr, "__iter", {}, [this](NFArgs && args){

//     }));
// }

// std::string IntRange::repr() const override {
//     // Here `obj_to_str` will exactly call `to_s`
//     return "<IntRange:"+ obj_to_str(shared_from_this()) +">";
// }

// yo_int IntRange::get_first() const {
//     return start;
// }

// yo_int IntRange::get_last() const {
//     return end;
// }