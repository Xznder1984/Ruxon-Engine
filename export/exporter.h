#pragma once

#include "ruxon/core/types.h"
#include <string>

namespace ruxon {
namespace export_ {

enum class Platform {
    Windows,
    Linux,
    macOS
};

class Exporter {
public:
    Exporter(Platform platform);
    bool Export(const std::string& projectPath, const std::string& outputPath);
    
private:
    Platform m_Platform;
};

} // namespace export_
} // namespace ruxon
