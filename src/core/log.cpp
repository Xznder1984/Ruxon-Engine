#include "ruxon/core/log.h"
#include <iostream>

namespace ruxon {
namespace core {

void Log::Info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

void Log::Warn(const std::string& msg) {
    std::cout << "[WARN] " << msg << std::endl;
}

void Log::Error(const std::string& msg) {
    std::cerr << "[ERROR] " << msg << std::endl;
}

} // namespace core
} // namespace ruxon
