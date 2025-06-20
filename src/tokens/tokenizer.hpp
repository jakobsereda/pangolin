#pragma once

#include <vector>
#include <string>
#include <functional>

#include "token.hpp"

class Tokenizer {
public:
	Tokenizer(std::string content)
		: content(std::move(content)), pos(0), row(1), col(1) {}

	std::vector<Token> tokenize();

private:
	std::string read_till(std::function<bool(char)> pred);
	bool        is_done() const;
	char        peek() const;
	char        eat();
	char		next_char() const;

	std::string content;
	std::size_t pos;
	std::size_t row;
	std::size_t col;
};
