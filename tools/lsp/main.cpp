#include "ruxon/core/log.h"
#include <iostream>

int main() {
    ruxon::core::Log::Info("Ruxon Language Server starting...");
    std::cout << "LSP server ready on stdio" << std::endl;
    return 0;
}
