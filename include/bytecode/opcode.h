#ifndef OPCODE_H
#define OPCODE_H

#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

#include <utility>
#include <vector>
#include <cstdint>
#include <map>

namespace jc::bytecode {
    struct Constant;
    using byte_list = std::vector<uint8_t>;
    using bytelist_it = byte_list::iterator;
    using constant_ptr = std::shared_ptr<Constant>;

    const int jump_space = 8;
    const uint8_t U255 = 0xFFu;
    const uint8_t U8 = 8u;

    /**
     * Bytecode spec
     * This bytecode is aimed at:
     * 1. Being evaluated as opcode object
     * 2. Being easy-writable/readable to/from file
     *
     * Bytecode file/list structure
     * -- (Here must be header info like version number, magic, etc.) --
     * constant_list_size
     * constant_list
     * code_size
     * code
     * attributes_count
     * attributes (names, line info, debug info)
     *
     * OPCODEs table
     * - CP - Constant Pool
     * - ST - Stack Top (ST = Stack[Stack.size - 1]. Can receive offset - ST[O] is Stack[Stack.size - O - 1])
     * - S - String constant offset
     * - L - Long (8-byte)
     *
     * -- Opcode --     | -- operands --
     * NOP              | [NothingType]
     * NullConst        | ST = NullConst
     * FalseConst       | ST = FalseConst
     * TrueConst        | ST = TrueConst
     * IntConst         | L -> ST = CP[L]
     * FloatConst       | L -> ST = CP[L]
     * StringConst      | L -> ST = CP[L]
     *
     * DefineGlobal     | S -> CP[S]
     * LoadGlobal       | S -> ST = globals[CP[S]]
     * StoreGlobal      | S -> globals[CP[S]] = ST
     * LoadLocal        | L -> ST = Frame.slots[L]
     * StoreLocal       | L -> Frame.slots[L] = ST
     *
     * Jump             | L -> IP += L
     * JumpFalse        | L -> ST is falsy ? IP += L
     *
     * Invoke           | L -> ST = Stack[L - 1].call(Stack[L..0])
     * InvokeMethod     | TODO: Add description
     *
     * GetProperty      | S -> ST = ST.getProperty(CP[S])
     * SetProperty      | S -> ST[1].setProperty(CP[A], ST)
     */

    enum class OpCode : uint8_t {
        NOP,
        Halt,
        Pop,

        NullConst,
        FalseConst,
        TrueConst,
        IntConst,
        FloatConst,
        StringConst,

        DefineGlobal,
        LoadGlobal,
        StoreGlobal,
        LoadLocal,
        StoreLocal,
        LoadUpvalue,
        StoreUpvalue,
        CloseUpvalue,
        Closure,

        Jump,
        JumpFalse,

        Return,

        Invoke,
        InvokeNF,
        InvokeMethod,
        InvokeNFMethod,

        GetProperty,
        SetProperty,
    };

    struct Chunk {
        // Global code
        byte_list code;

        std::vector<constant_ptr> constant_pool;
//        std::vector<Attribute> attributes;
    };
}

#endif
