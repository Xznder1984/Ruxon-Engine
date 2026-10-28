#include "ruxon/scripting/script.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace scripting {

bool Script::Compile() {
    core::Log::Info("Compiling Ruxon Script...");
    m_Compiled = true;
    return true;
}

void Script::Execute() {
    if (!m_Compiled) Compile();
    core::Log::Info("Executing Ruxon Script...");
}

} // namespace scripting
} // namespace ruxon
