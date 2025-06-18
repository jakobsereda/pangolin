#include "tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize()
{
	std::vector<Token> tokens;

	while (!is_done()) {
		char curr = peek();

		if (std::isspace(curr)) {
			eat();
			continue;
		}

		if (std::isalpha(curr)) {
			std::string ident = read_till([](char c) {
				return !std::isalnum(c) && c != '_';
			});

			if (ident == "let") {
				tokens.push_back({ TokenType::Let, ident, row, col });
			} else {
				tokens.push_back({ TokenType::Identifier, ident, row, col });
			}

			continue;
		}

		if (std::isdigit(curr)) {
			std::string num = read_till([](char c) {
				return !std::isdigit(c);
			});

			tokens.push_back(Token { TokenType::Integer, num, row, col });
			continue;
		}

		switch (curr) {
			case '=': tokens.push_back({ TokenType::Equals,   "=", row, col }); break;
			case '+': tokens.push_back({ TokenType::Plus,     "+", row, col }); break;
			case '-': tokens.push_back({ TokenType::Minus,    "-", row, col }); break;
			case '*': tokens.push_back({ TokenType::Multiply, "*", row, col }); break;
			case '/': tokens.push_back({ TokenType::Divide,   "/", row, col }); break;
			case '%': tokens.push_back({ TokenType::Modulus,  "%", row, col }); break;
			case '&': tokens.push_back({ TokenType::BAnd,     "&", row, col }); break;
			case '|': tokens.push_back({ TokenType::BOr,      "|", row, col }); break;
			case '^': tokens.push_back({ TokenType::BXor,     "^", row, col }); break;
			case '~': tokens.push_back({ TokenType::BNot,     "~", row, col }); break;
			case '(': tokens.push_back({ TokenType::LParen,   "(", row, col }); break;
			case ')': tokens.push_back({ TokenType::RParen,   ")", row, col }); break;
			default:  tokens.push_back({ TokenType::Invalid,  std::string(1, curr), row, col });
		}

		eat();
	}

	return tokens;
}

std::string Tokenizer::read_till(std::function<bool(char)> pred)
{
	std::string res;

	while (!is_done()) {
		char c = peek();
		if (pred(c)) break;
		res += eat();
	}

	return res;
}

bool Tokenizer::is_done() const
{
	return pos >= content.size();
}

char Tokenizer::peek() const
{
	return is_done() ? '\0' : content[pos];
}

char Tokenizer::eat()
{
	if (is_done()) return '\0';

	char c = content[pos++];
	if (c == '\n') {
		row++;
		col = 1;
	} else {
		col++;
	}

	return c;
}
