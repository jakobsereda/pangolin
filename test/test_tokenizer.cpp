#include "doctest.h"
#include "../src/tokens/tokenizer.hpp"

TEST_CASE("comparison ops test")
{
    Tokenizer tokenizer("<> == <= >=");
	std::vector<Token> tokens = tokenizer.tokenize();
    CHECK(tokens[0].type == TokenType::Less);
    CHECK(tokens[1].type == TokenType::Greater);
    CHECK(tokens[2].type == TokenType::EqualEqual);
    CHECK(tokens[3].type == TokenType::LessEqual);
    CHECK(tokens[4].type == TokenType::GreaterEqual);
}

TEST_CASE("math ops and binary ops test")
{
    Tokenizer tokenizer("+-/* = % & ^ | ~");
	std::vector<Token> tokens = tokenizer.tokenize();
    CHECK(tokens[0].type == TokenType::Plus);
    CHECK(tokens[1].type == TokenType::Minus);
    CHECK(tokens[2].type == TokenType::Divide);
    CHECK(tokens[3].type == TokenType::Multiply);
    CHECK(tokens[4].type == TokenType::Equals);
    CHECK(tokens[5].type == TokenType::Modulus);
    CHECK(tokens[6].type == TokenType::BAnd);
    CHECK(tokens[7].type == TokenType::BXor);
    CHECK(tokens[8].type == TokenType::BOr);
    CHECK(tokens[9].type == TokenType::BNot);
}

TEST_CASE("if statements test")
{
    Tokenizer tokenizer("if (a == 4) {} else if (a == 5) {} else if (a == 9) else {}");
	std::vector<Token> tokens = tokenizer.tokenize();
    CHECK(tokens[0].type == TokenType::If);
    CHECK(tokens[8].type == TokenType::Else);
    CHECK(tokens[9].type == TokenType::If);
}
