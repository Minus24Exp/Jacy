#ifndef DISASM_H
#define DISASM_H

#include "vm/BaseVM.h"
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
    {OpCode::GetProperty, "GetProperty"},
    {OpCode::SetProperty, "SetProperty"},
};

struct DisasmOptions {
    // Note: Default options
    bool pure_dump{true};
};

class Disasm : public BaseVM {
public:
    Disasm();
    explicit Disasm(const DisasmOptions & options);
    ~Disasm() override = default;

    void eval(const Chunk & chunk) override;

private:
    DisasmOptions options{};
};

#endif // DISASM_H
