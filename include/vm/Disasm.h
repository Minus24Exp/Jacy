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
    {OpCode::Call, "Call"},
    {OpCode::GetProperty, "GetProperty"},
    {OpCode::SetProperty, "SetProperty"},
};

class Disasm : public BaseVM {
public:
    Disasm();
    ~Disasm() override = default;

    void eval(const Chunk & chunk) override;
};

#endif // DISASM_H
