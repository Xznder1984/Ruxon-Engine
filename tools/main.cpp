#include "ruxon/core/log.h"
#include <iostream>

void PrintUsage() {
    std::cout << "Ruxon CLI\n";
    std::cout << "Usage: ruxon <command>\n\n";
    std::cout << "Commands:\n";
    std::cout << "  new       Create a new project\n";
    std::cout << "  run       Run a project\n";
    std::cout << "  build     Build a project\n";
    std::cout << "  export    Export a project\n";
    std::cout << "  check     Check project for errors\n";
    std::cout << "  format    Format Ruxon Script files\n";
    std::cout << "  docs      Open documentation\n";
    std::cout << "  lsp       Start language server\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        PrintUsage();
        return 0;
    }
    
    std::string cmd = argv[1];
    if (cmd == "new") {
        ruxon::core::Log::Info("Creating new project...");
    } else if (cmd == "run") {
        ruxon::core::Log::Info("Running project...");
    } else if (cmd == "build") {
        ruxon::core::Log::Info("Building project...");
    } else if (cmd == "export") {
        ruxon::core::Log::Info("Exporting project...");
    } else if (cmd == "check") {
        ruxon::core::Log::Info("Checking project...");
    } else if (cmd == "format") {
        ruxon::core::Log::Info("Formatting scripts...");
    } else if (cmd == "docs") {
        ruxon::core::Log::Info("Opening docs...");
    } else if (cmd == "lsp") {
        ruxon::core::Log::Info("Starting LSP server...");
    } else {
        PrintUsage();
    }
    
    return 0;
}
