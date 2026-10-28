#pragma once

#include "ruxon/core/types.h"
#include <string>

namespace ruxon {
namespace scripting {

class Script {
public:
    Script(const std::string& source) : m_Source(source) {}
    
    bool Compile();
    void Execute();
    
private:
    std::string m_Source;
    bool m_Compiled = false;
};

} // namespace scripting
} // namespace ruxon
