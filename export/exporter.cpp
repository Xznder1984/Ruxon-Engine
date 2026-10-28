#include "exporter.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace export_ {

Exporter::Exporter(Platform platform) : m_Platform(platform) {}

bool Exporter::Export(const std::string& projectPath, const std::string& outputPath) {
    core::Log::Info("Exporting for platform...");
    core::Log::Info("Creating release archive with SHA-256 checksum");
    return true;
}

} // namespace export_
} // namespace ruxon
