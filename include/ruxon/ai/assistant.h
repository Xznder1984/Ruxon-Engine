#pragma once

#include "ruxon/core/types.h"
#include <string>
#include <vector>

namespace ruxon {
namespace ai {

class AIAssistant {
public:
    AIAssistant();
    ~AIAssistant();
    
    void SetApiKey(const std::string& key);
    bool HasApiKey() const;
    
    std::string Chat(const std::string& message, const std::vector<std::string>& context);
    
    void ShowPrivacyNotice();
    
private:
    std::string m_ApiKey;
    bool m_Enabled = false;
};

} // namespace ai
} // namespace ruxon
