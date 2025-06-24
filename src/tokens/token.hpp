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
	For,
	Semicolon,
	Invalid
};

struct Token {
	TokenType   type;
	std::string literal;
	std::size_t row;
	std::size_t col;
};

// inline std::ostream& operator<<(std::ostream& os, const Token& token) {
//     os << "{type: " << static_cast<int>(token.type)
//        << ", literal: \"" << token.literal
//        << "\", row: " << token.row
//        << ", col: " << token.col << "}";
//     return os;
// }