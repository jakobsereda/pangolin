#pragma once

#include <vector>
#include <string>

enum class TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END,
    INVALID
};

struct Token {
    TokenType   type;
    std::string literal;
};

class Tokenizer {
public:
    Tokenizer(const std::string& input);
    std::vector<Token> tokenize();
    
private:
    size_t pos;
    std::string input;

    std::string readNumber();
};
