#pragma once

#include "ruxon/core/types.h"
#include <vector>

namespace ruxon {
namespace scripting {

enum class OpCode {
    LoadConst,
    Add,
    Sub,
    Mul,
    Div,
    Call,
    Return,
    Halt
};

struct Instruction {
    OpCode opcode;
    u32 operand;
};

class VM {
public:
    VM();
    void Execute(const std::vector<Instruction>& code);
    
private:
    std::vector<f64> m_Stack;
};

} // namespace scripting
} // namespace ruxon
