#include <algorithm>

#include "parser.hpp"

/**
 * Check if current token matches the given type, and if so, consume it
 * @param type The token type to match against
 * @return true if token matched and was consumed, false otherwise
 */
bool Parser::match(TokenType type)
{
	if (check(type)) {
		eat();
		return true;
	}

	return false;
}

/**
 * Check if current token matches the given type without consuming it
 * @param type The token type to check against
 * @return true if current token matches type, false otherwise
 */
bool Parser::check(TokenType type) const
{
	if (is_done()) return false;
	return peek().type == type;
}

bool Parser::is_done() const
{
	// We could introduce an end token again
	// and check for that here instead if you
	// wanted. I don't see a need to right now,
	// especially since the end token was doubling
	// as an invalid token before, so just having
	// invalid for now seems reasonable.
	return curr >= tokens.size();
}

/**
 * @return Current token at "current" index (bad wording whoops)
 */
Token Parser::peek() const
{
	return is_done()
		? Token { TokenType::Invalid, "", 0, 0 }
		: tokens[curr];
}

/**
 * Consume current token, and move onto the next one.
 * @return the consumed token
 */
Token Parser::eat()
{
	return is_done()
		? Token { TokenType::Invalid, "", 0, 0 }
		: tokens[curr++];
}

std::unique_ptr<ASTNode> Parser::parse_program()
{
	auto program = std::make_unique<ProgramNode>();

	while (!is_done()) {
		std::unique_ptr<ASTNode> statement = parse_statement();
		program->add_statement(std::move(statement));
	}

	return program;
}

std::unique_ptr<ASTNode> Parser::parse_statement()
{
	if (check(TokenType::Let))
		return parse_assignment();
	
	if (check(TokenType::If))
		return parse_if_statement();
	
	if (check(TokenType::LBrace))
		return parse_block();
	
	std::unique_ptr<ASTNode> expr = parse_expression();
	
	// expect semicolon after expression statements
	if (!match(TokenType::Semicolon)) {
		return std::make_unique<ErrorNode>("Expected ';' after expression statement");
	}
	
	return expr;
}

std::unique_ptr<ASTNode> Parser::parse_assignment()
{
	Token let_token = eat();

	// I wanted to use match here but then you
	// run into issues since the AssignmentNode
	// constructor takes in the Identifier...
	// This works for now but it's not elegant.
	if (!check(TokenType::Identifier)) {
		eat();
		return std::make_unique<ErrorNode>("Expected identifier in assignment statement");
	}

	Token ident = eat();

	if (!match(TokenType::Equals))
		return std::make_unique<ErrorNode>("Expected '=' to be used for assignment");

	std::unique_ptr<ASTNode> value = parse_expression();

	// expect semicolons after assignments
	if (!match(TokenType::Semicolon)) {
		return std::make_unique<ErrorNode>("Expected ';' after assignment.")
	}

	return std::make_unique<AssignmentNode>(ident, std::move(value));
}

std::unique_ptr<ASTNode> Parser::parse_if_statement()
{
    Token if_token = eat(); // consume 'if'

    if (!match(TokenType::LParen))
        return std::make_unique<ErrorNode>("Expected '(' after 'if'");

    std::unique_ptr<ASTNode> condition = parse_expression();

    if (!match(TokenType::RParen))
        return std::make_unique<ErrorNode>("Expected ')' after if condition");

    std::unique_ptr<ASTNode> then_branch = parse_statement();

    std::unique_ptr<ASTNode> else_branch = nullptr;
    if (match(TokenType::Else)) {
        if (check(TokenType::If)) {
            else_branch = parse_if_statement(); // Recursively parse 'else if'
        } else {
            else_branch = parse_statement(); // Regular else
        }
    }

    return std::make_unique<IfNode>(std::move(condition), std::move(then_branch), std::move(else_branch));
}

std::unique_ptr<ASTNode> Parser::parse_block()
{
	if (!match(TokenType::LBrace))
		return std::make_unique<ErrorNode>("Expected '{'");

	auto block = std::make_unique<BlockNode>();

	while (!check(TokenType::RBrace) && !is_done()) {
		std::unique_ptr<ASTNode> statement = parse_statement();
		block->add_statement(std::move(statement));
	}

	if (!match(TokenType::RBrace))
		return std::make_unique<ErrorNode>("Expected '}'");

	return block;
}

std::unique_ptr<ASTNode> Parser::parse_expression()
{
	return parse_comparison_expr();
}

std::unique_ptr<ASTNode> Parser::parse_comparison_expr()
{
	return parse_left_associative_binary_expr(
		[this]() { return parse_bitwise_or_expr(); },
		{
			TokenType::Less,
			TokenType::Greater,
			TokenType::EqualEqual,
			TokenType::NotEqual,
			TokenType::LessEqual,
			TokenType::GreaterEqual
		}
	);
}

std::unique_ptr<ASTNode> Parser::parse_bitwise_or_expr()
{
	return parse_left_associative_binary_expr(
		[this]() { return parse_bitwise_xor_expr(); },
		{ TokenType::BOr }
	);
}

std::unique_ptr<ASTNode> Parser::parse_bitwise_xor_expr()
{
	return parse_left_associative_binary_expr(
		[this]() { return parse_bitwise_and_expr(); },
		{ TokenType::BXor }
	);
}

std::unique_ptr<ASTNode> Parser::parse_bitwise_and_expr()
{
	return parse_left_associative_binary_expr(
		[this]() { return parse_additive_expr(); },
		{ TokenType::BAnd }
	);
}

std::unique_ptr<ASTNode> Parser::parse_additive_expr()
{
	return parse_left_associative_binary_expr(
		[this]() { return parse_multiplicative_expr(); },
		{ TokenType::Plus, TokenType::Minus }
	);
}

std::unique_ptr<ASTNode> Parser::parse_multiplicative_expr()
{
	return parse_left_associative_binary_expr(
		[this]() { return parse_unary_expr(); },
		{ TokenType::Multiply, TokenType::Divide, TokenType::Modulus }
	);
}

std::unique_ptr<ASTNode> Parser::parse_unary_expr()
{
	if (check(TokenType::BNot) || check(TokenType::Minus)) {
		Token op = eat();
		std::unique_ptr<ASTNode> body = parse_primary_expr();
		return std::make_unique<UnaryOpNode>(op, std::move(body));
	}

	return parse_primary_expr();
}

std::unique_ptr<ASTNode> Parser::parse_primary_expr()
{
	// These can both be handled the same way
	// for now, but we probably will have/want
	// to split them up eventually.
	if (check(TokenType::Integer) || check(TokenType::Identifier)) {
		Token lit = eat();
		return std::make_unique<LiteralNode>(lit);
	}

	if (match(TokenType::LParen)) {
		std::unique_ptr<ASTNode> body = parse_expression();
		if (match(TokenType::RParen))
			return std::make_unique<ParenthesesNode>(std::move(body));
		return std::make_unique<ErrorNode>("Expected closing parentheses");
	}

	return std::make_unique<ErrorNode>("Invalid primary expression");
}

std::unique_ptr<ASTNode> Parser::parse_left_associative_binary_expr(
	std::function<std::unique_ptr<ASTNode>()> next,
	const std::vector<TokenType>& ops)
{
	std::unique_ptr<ASTNode> left = next();

	for (;;) {
		auto it = std::find_if(
			ops.begin(),
			ops.end(),
			[this](TokenType t) { return check(t); }
		);

		if (it == ops.end())
			break;

		Token op = eat();
		std::unique_ptr<ASTNode> right = next();
		left = std::make_unique<BinaryOpNode>(
			op,
			std::move(left),
			std::move(right)
		);
	}

	return left;
}
