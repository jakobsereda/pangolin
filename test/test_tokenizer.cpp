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