#pragma once

#include "ruxon/core/types.h"
#include <string>
#include <vector>

namespace ruxon {
namespace scripting {

enum class TokenType {
    Eof,
    Identifier,
    Number,
    String,
    Keyword,
    Symbol,
    Whitespace
};

struct Token {
    TokenType type;
    std::string value;
    u32 line;
    u32 column;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> Tokenize();
    
private:
    std::string m_Source;
    u32 m_Position = 0;
    u32 m_Line = 1;
    u32 m_Column = 1;
};

} // namespace scripting
} // namespace ruxon
