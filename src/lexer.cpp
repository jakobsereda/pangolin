#include "lexer.hpp"

Lexer::Lexer (const std::string& input) : input(input), pos(0) {}

std::string Lexer::readNumber() {
    size_t start = pos;
    while (pos < input.length() && std::isdigit(input[pos])){
        ++pos;
    }
    return input.substr(start, pos - start);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while(pos < input.length()){
        char current = input[pos];

        if(std::isspace(current)){
            ++pos;
            continue;
        }

        // what hte sigma
        if(std::isdigit(current)){
            tokens.push_back(Token{TokenType::NUMBER, readNumber()});
        } else if (current == '+') {
            tokens.push_back(Token{TokenType::PLUS, "+"});
            ++pos;
        } else if (current == '-') {
            tokens.push_back(Token{TokenType::MINUS, "-"});
            ++pos;
        } else if (current == '*') {
            tokens.push_back(Token{TokenType::MULTIPLY, "*"});
            ++pos;
        } else if (current == '/') {
            tokens.push_back(Token{TokenType::DIVIDE, "/"});
            ++pos;
        } else if (current == '(') {
            tokens.push_back(Token{TokenType::LPAREN, "("});
            ++pos;
        } else if (current == ')') {
            tokens.push_back(Token{TokenType::RPAREN, ")"});
            ++pos;
        } else {
            tokens.push_back(Token{TokenType::INVALID, std::string(1, current)});
            ++pos;
        }
    }

    tokens.push_back(Token{TokenType::END, ""});
    return tokens;
}