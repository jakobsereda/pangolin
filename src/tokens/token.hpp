#pragma once

#include <string>

enum TokenType {
	Let,
	Identifier,
	Integer,
	Equals,
	Plus,
	Minus,
	Multiply,
	Divide,
	Modulus,
	BAnd,
	BOr,
	BXor,
	BNot,
	LParen,
	RParen,
	LBrace,
	RBrace,
	Less,
	Greater,
	EqualEqual,
	NotEqual,
	LessEqual,
	GreaterEqual,
	If,
	Else,
	Invalid
};

struct Token {
	TokenType   type;
	std::string literal;
	std::size_t row;
	std::size_t col;
};
