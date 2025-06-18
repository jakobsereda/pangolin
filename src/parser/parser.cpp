#include "parser.hpp"
#include <stdexcept>
#include <iostream>

/**
 * Constructor: initialize parser with token vector and set current position to 0
 */
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

/**
 * @return Current token at "current" index (bad wording whoops)
 */
Token Parser::peek() const {
    return tokens[current];
}

/**
 * Consume current token, and move onto the next one.
 * @return the consumed token
 */
Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

/**
 * Check if current token matches the given type, and if so, consume it
 * @param type The token type to match against
 * @return true if token matched and was consumed, false otherwise
 */
bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

/**
 * Check if current token matches the given type without consuming it
 * @param type The token type to check against
 * @return true if current token matches type, false otherwise
 */
bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

/**
 * Check if we've reached the end of the token stream
 * @return true if current token is END token, false otherwise
 */
bool Parser::isAtEnd() const {
    return peek().type == TokenType::Invalid;
}

/**
 * "Entry point" for parsing, starts the recursive descent process
 * @return Root AST node on success, nullptr on parsing error
 */
std::unique_ptr<ASTNode> Parser::parse() {
    try {
        return expression();
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return nullptr;
    }
}

/**
 * Parse addition and subtraction expressions (lowest precedence)
 * This implements left-associative parsing, so "1 + 2 - 3" is parsed as "(1 + 2) - 3"
 * @return AST node representing the full addition/subtraction expression
 */
std::unique_ptr<ASTNode> Parser::expression() {
    auto expr = term();

    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        Token op = tokens[current - 1];
        auto right = term();
        expr = std::make_unique<BinaryOpNode>(
            std::move(expr), 
            std::move(right), 
            op.literal[0]
        );
    }

    return expr;
}

/**
 * Parse multiplication and division expressions (higher precedence than +/-)
 * @return AST node representing the multiplication/division expression
 */
std::unique_ptr<ASTNode> Parser::term() {
    auto expr = unary();

    while (match(TokenType::Multiply) || match(TokenType::Divide)) {
        Token op = tokens[current - 1];
        auto right = unary();
        expr = std::make_unique<BinaryOpNode>(
            std::move(expr), 
            std::move(right), 
            op.literal[0]
        );
    }

    return expr;
}

/**
 * Parse unary expressions (highest precedence) 
 * @return AST node representing unary expression or primary expression
 */
std::unique_ptr<ASTNode> Parser::unary() {
    if (match(TokenType::Minus)) {
        auto expr = unary();
        return std::make_unique<UnaryMinusNode>(std::move(expr));
    }

    return primary();
}

/**
 * Parse primary expressions - the base case of the recursion
 * 
 * Handles:
 * 1. Number literals
 * 2. Parenthesized expressions (like "(2 + 3)")
 *  
 * @return AST node representing a number or parenthesized expression
 */
std::unique_ptr<ASTNode> Parser::primary() {
    if (match(TokenType::Integer)) {
        Token numToken = tokens[current - 1];
        double value = std::stod(numToken.literal);
        return std::make_unique<NumberNode>(value);
    }

    if (match(TokenType::LParen)) {
        auto expr = expression();
        if (!match(TokenType::RParen)) {
            throw std::runtime_error("Expected ')' after expression");
        }
        return expr;
    }

    throw std::runtime_error("Expected expression");
}
