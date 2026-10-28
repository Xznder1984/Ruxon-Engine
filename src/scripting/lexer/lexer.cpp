#include "ruxon/scripting/lexer.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace scripting {

Lexer::Lexer(const std::string& source) : m_Source(source) {}

std::vector<Token> Lexer::Tokenize() {
    core::Log::Info("Tokenizing Ruxon Script...");
    std::vector<Token> tokens;
    // Simple implementation
    Token eofToken = {TokenType::Eof, "", m_Line, m_Column};
    tokens.push_back(eofToken);
    return tokens;
}

} // namespace scripting
} // namespace ruxon
