#include "ruxon/scripting/vm.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace scripting {

VM::VM() {}

void VM::Execute(const std::vector<Instruction>& code) {
    core::Log::Info("Executing bytecode in Ruxon VM...");
    for (const auto& instr : code) {
        // Execute instruction
    }
    core::Log::Info("VM execution complete");
}

} // namespace scripting
} // namespace ruxon
