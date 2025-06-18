#pragma once

#include <vector>
#include <string>
#include <functional>

#include "token.hpp"

class Tokenizer {
public:
    Tokenizer(const std::string &content)
		: content(content), pos(0), row(1), col(1) {}

	std::vector<Token> tokenize();

private:
	std::string read_till(std::function<bool(char)> pred);
	bool        is_done();
	char        peek();
	char        eat();

	std::string content;
	std::size_t pos;
	std::size_t row;
	std::size_t col;
};
