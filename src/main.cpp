#include "tokens/tokenizer.hpp"
#include "parser/parser.hpp"
#include "parser/ast.hpp"
#include <iostream>
#include <string>

/**
 * I have replaced the main file for now, so we can interact with our code in the cli
 */
int main() {
    std::string input;
    
    std::cout << "Simple Expression Calculator" << std::endl;
    std::cout << "Enter expressions (or 'quit' to exit):" << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        
        if (input.empty()) {
            continue;
        }
        
        try {
            Tokenizer tokenizer(input);
            auto tokens = tokenizer.tokenize();
            
            Parser parser(tokens);
            auto ast = parser.parse();
            
            if (ast) {
                double result = ast->evaluate();
                std::cout << "Result: " << result << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}

/*
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
 */