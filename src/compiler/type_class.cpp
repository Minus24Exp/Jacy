#include "compiler/type_class.h"

namespace jc::compiler {
    ///////////
    // Types //
    ///////////

    /////////////
    // Helpers //
    /////////////


    func_t_ptr class_has_method(const type_ptr & type, const std::string & method_name, const func_t_ptr & signature, bool is_op_optional) {
        // TODO: Improve for most inherited types,
        //  for a(int) call must be used a(int) if exists, not a(any)

        const auto & eq_range = type->methods.equal_range(method_name);

        for (auto it = eq_range.first; it != eq_range.second; it++) {
            if (it->second->compare(signature, is_op_optional)) {
                return it->second;
            }
        }

        return nullptr;
    }
}
