#include <iostream>
#include <fstream>
#include <string>

#include "tokens/tokenizer.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "USAGE: pangolin <filepath>" << '\n';
        return 1;
    }

    std::ifstream file { argv[1] };
    std::string data {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
    file.close();

    Tokenizer t(data);
    std::vector<Token> tokens = t.tokenize();
    
    for (auto t : tokens) {
        switch (t.type) {
            case TokenType::NUMBER: std::cout << "NUMBER\n"; break;
            case TokenType::PLUS: std::cout << "PLUS\n"; break;
            case TokenType::MINUS: std::cout << "MINUS\n"; break;
            case TokenType::MULTIPLY: std::cout << "MUL\n"; break;
            case TokenType::DIVIDE: std::cout << "DIV\n"; break;
            case TokenType::LPAREN: std::cout << "LPAREN\n"; break;
            case TokenType::RPAREN: std::cout << "RPAREN\n"; break;
			case TokenType::END: std::cout << "END\n"; break;
			case TokenType::INVALID: std::cout << "INVALID\n"; break;
        }
    }
    
    return 0;
}
