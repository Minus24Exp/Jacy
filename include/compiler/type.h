#ifndef TYPE_H
#define TYPE_H

#include "tree/nodes.h"
#include "Exception.h"

#include <string>
#include <memory>
#include <map>

namespace jc::compiler {
    // TODO: ! Move all structs to top and constants to bottom

    struct Type;
    struct FuncType;
    struct FuncParamType;
    using type_ptr = std::shared_ptr<Type>;
    using t_list = std::vector<type_ptr>;
    using func_t_ptr = std::shared_ptr<FuncType>;
    using func_param_t_ptr = std::shared_ptr<FuncParamType>;
    using func_param_t_list = std::vector<func_param_t_ptr>;

    func_t_ptr class_has_method(const type_ptr & type, const std::string & method_name, const func_t_ptr & signature, bool is_op_optional);

    enum class TypeTag {
        None,
        Bottom,
        Unit,
        Null,
        Bool,
        Int,
        Float,
        String,
        List,
        Dict,
        Generic,
        Func,
        NativeFunc,
        Class,
        VarargTag,
        Union,
        Any,
    };

    struct Field {
        VarDeclKind kind;
        type_ptr type;
//        std::string name;
    };

    struct Type {
        explicit Type(TypeTag tag);

        TypeTag tag{TypeTag::None};
        std::string name;
        std::map<std::string, Field> fields;
        std::map<std::string, func_t_ptr> methods;

        virtual bool equals(const type_ptr & other) = 0;
        virtual std::string to_string() = 0;

        // Mangling //
        std::string mangle_name(const std::string & t_name);
        virtual std::string mangle() = 0;

        func_t_ptr has_method(const std::string & method_name, const func_t_ptr & signature, bool is_op_optional);
    };

    struct Nothing : Type {
        Nothing();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct UnitType : Type {
        UnitType();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct NullType : Type {
        NullType();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct BoolType : Type {
        BoolType();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct IntType : Type {
        IntType();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct FloatType : Type {
        FloatType();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct StringType : Type {
        StringType();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct FuncParamType : Type {
        explicit FuncParamType(const type_ptr & type, bool has_default_val = false);

        type_ptr type;
        // Note: has_default_val does not matter for type comparison, signature search and etc.
        //  The only case is check in function call. default is false
        bool has_default_val;

        static func_param_t_ptr get(const type_ptr & type, bool has_default_val = false);

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct FuncType : Type {
        FuncType(const type_ptr & return_type,
                 const func_param_t_list & arg_types,
                 bool is_operator = false,
                 TypeTag callable_type = TypeTag::Func);

        type_ptr return_type;
        // TODO: Default values
        func_param_t_list arg_types;
        bool is_operator{false};

        static func_t_ptr get(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator = false, TypeTag callable_type = TypeTag::Func);
        static func_t_ptr get_nf_t(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator = false);
        static func_t_ptr get_nf_op_t(const type_ptr & return_type, const func_param_t_list & arg_types);

        bool equals(const type_ptr & other) override;

        // Note: Use this function mostly always instead of overridden compare
        bool compare(const type_ptr & other, bool is_op_optional);

        // FuncType-FuncType comparison (may be used for similar function searching)
        bool compare(const type_ptr & _return_type, const func_param_t_list & other_arg_types);

        // FuncCall comparison
        bool compare(const func_param_t_list & other_arg_types);

        std::string to_string() override;
        std::string mangle() override;
    };

    struct Any : Type {
        Any();

        static type_ptr get();

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct VarargType : Type {
        explicit VarargType(const type_ptr & vararg_type);

        type_ptr vararg_type;

        static type_ptr get(const type_ptr & vararg_type);

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct ListType : Type {
        explicit ListType(const type_ptr & type);

        type_ptr type;

        static type_ptr get(const type_ptr & type);

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct DictType : Type {
        DictType(const type_ptr & key, const type_ptr & val);

        type_ptr key;
        type_ptr val;

        static type_ptr get(const type_ptr & key, const type_ptr & value);

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct GenericType : Type {
        GenericType(const type_ptr & generic, const t_list & types);

        type_ptr generic;
        t_list types;

        static type_ptr get(const type_ptr & generic, const t_list & types);

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };

    struct UnionType : Type {
        explicit UnionType(const t_list & types);

        t_list types;

        static type_ptr get(const t_list & types);
        static type_ptr get_nullable_t(const type_ptr & type);

        bool equals(const type_ptr & other) override;
        std::string to_string() override;
        std::string mangle() override;
    };
}

#endif // TYPE_CLASS_H
