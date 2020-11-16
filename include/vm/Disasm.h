#ifndef DISASM_H
#define DISASM_H

#include "vm/BaseVM.h"
#include "common/Logger.h"
#include <map>
#include <cstring>

const std::map<OpCode, std::string> opcode_names = {
    {OpCode::NOP, "NOP"},
    {OpCode::Pop, "POP"},
    {OpCode::NullConst, "NullConst"},
    {OpCode::FalseConst, "FalseConst"},
    {OpCode::TrueConst, "TrueConst"},
    {OpCode::IntConst, "IntConst"},
    {OpCode::FloatConst, "FloatConst"},
    {OpCode::StringConst, "StringConst"},
    {OpCode::DefineGlobal, "DefineGlobal"},
    {OpCode::LoadGlobal, "LoadGlobal"},
    {OpCode::StoreGlobal, "StoreGlobal"},
    {OpCode::LoadLocal, "LoadLocal"},
    {OpCode::StoreLocal, "StoreLocal"},
    {OpCode::Jump, "Jump"},
    {OpCode::JumpFalse, "JumpFalse"},
    {OpCode::Invoke, "Invoke"},
    {OpCode::InvokeNF, "InvokeNF"},
    {OpCode::InvokeMethod, "InvokeMethod"},
    {OpCode::GetProperty, "GetProperty"},
    {OpCode::SetProperty, "SetProperty"},
};

class Disasm : public BaseVM {
public:
    Disasm();
    explicit Disasm(const DisasmOptions & options);
    ~Disasm() override = default;

    void before_eval() override;
    void before_opcode(OpCode opcode) override;
    void after_opcode() override;
//    void unknown_opcode() override;
    void _nop() override;
    void _pop() override;
    void _null_const() override;
    void _false_const() override;
    void _true_const() override;
    void _int_const() override;
    void _float_const() override;
    void _string_const() override;
    void _define_global() override;
    void _load_global() override;
    void _store_global() override;
    void _load_local() override;
    void _store_local() override;
    void _jump() override;
    void _jump_false() override;
    void _invoke() override;
    void _invoke_nf() override;
    void _invoke_method() override;
    void _get_property() override;
    void _set_property() override;

    // Debug //
public:
    DisasmOptions options{};
    Logger log;
};

#endif // DISASM_H
