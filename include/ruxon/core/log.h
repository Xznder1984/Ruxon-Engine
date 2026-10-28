#pragma once

#include <string>

namespace ruxon {
namespace core {

class Log {
public:
    static void Info(const std::string& msg);
    static void Warn(const std::string& msg);
    static void Error(const std::string& msg);
};

} // namespace core
} // namespace ruxon
