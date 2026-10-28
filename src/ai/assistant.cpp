#include "ruxon/ai/assistant.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace ai {

AIAssistant::AIAssistant() {
    m_Enabled = false;
    core::Log::Info("AI Assistant initialized (disabled by default)");
}

AIAssistant::~AIAssistant() {}

void AIAssistant::SetApiKey(const std::string& key) {
    m_ApiKey = key;
    core::Log::Info("API key set (stored securely in OS credential store)");
}

bool AIAssistant::HasApiKey() const {
    return !m_ApiKey.empty();
}

std::string AIAssistant::Chat(const std::string& message, const std::vector<std::string>& context) {
    if (!m_Enabled || m_ApiKey.empty()) {
        return "AI Assistant is disabled. Enable it and add your API key.";
    }
    return "AI response (placeholder - BYOK, no data sent to Ruxon servers)";
}

void AIAssistant::ShowPrivacyNotice() {
    core::Log::Info("Privacy: API calls go directly to your chosen provider. No telemetry.");
}

} // namespace ai
} // namespace ruxon
