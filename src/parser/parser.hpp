#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "../tokens/token.hpp"
#include "ast.hpp"

class Parser {
public:
	Parser(std::vector<Token> tokens)
		: tokens(std::move(tokens)), curr(0) {}

	std::unique_ptr<ASTNode> parse_program();

private:
	std::vector<Token> tokens;
	size_t             curr;

	bool  match(TokenType type);
	bool  check(TokenType type) const;
	bool  is_done() const;
	Token peek() const;
	Token eat();

	std::unique_ptr<ASTNode> parse_statement();
	std::unique_ptr<ASTNode> parse_assignment();
	std::unique_ptr<ASTNode> parse_expression();
	std::unique_ptr<ASTNode> parse_bitwise_or_expr();
	std::unique_ptr<ASTNode> parse_bitwise_xor_expr();
	std::unique_ptr<ASTNode> parse_bitwise_and_expr();
	std::unique_ptr<ASTNode> parse_additive_expr();
	std::unique_ptr<ASTNode> parse_multiplicative_expr();
	std::unique_ptr<ASTNode> parse_unary_expr();
	std::unique_ptr<ASTNode> parse_primary_expr();
	std::unique_ptr<ASTNode> parse_comparison_expr();
	std::unique_ptr<ASTNode> parse_if_statement();
	std::unique_ptr<ASTNode> parse_for_statement();
	std::unique_ptr<ASTNode> parse_block();

	std::unique_ptr<ASTNode> parse_left_associative_binary_expr(
		std::function<std::unique_ptr<ASTNode>()> next,
		const std::vector<TokenType>& ops
	);
};
