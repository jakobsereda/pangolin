#pragma once
#include "../tokens/tokenizer.hpp"
#include "ast.hpp"
#include <memory>

/** 
 * This class implements a recursive descent parser for only math expressions (for now).
 * 
 * Grammar (in order of precedence, lowest -> highest):
 * expression -> term ( ("+" | "-" ) ) term )*
 * term       -> factor ( ("*" | "/") ) term )*
 * factory    -> unary
 * unary      -> ( "-" ) unary | primary
 * primary    -> NUMBER | "( expression )"
 * 
 * This grammar ensures proper operator precedence:
 * - Addition/Subtraction (lowest precedence)
 * - Multiplication/Division (higher precedence)
 * - Unary operators (highest precedence)
 * - Parentheses override precedence
 */
class Parser {
private:
    std::vector<Token> tokens;
    size_t current;

    Token peek() const;
    Token advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    bool isAtEnd() const;

    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> term();
    std::unique_ptr<ASTNode> unary();
    std::unique_ptr<ASTNode> primary();

public:
    /**
     * Constructor: initialize parser with tokens from the tokenizer
     * @params tokens Vector of tokens to parse
     */
    Parser(const std::vector<Token>& tokens);

    /**
     *
     * Parses tokens into an AST
     * @return Root node of the abstract syntax tree
     */
    std::unique_ptr<ASTNode> parse();
};