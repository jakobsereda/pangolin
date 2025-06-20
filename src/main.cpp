#include <iostream>
#include <fstream>
#include <string>

#include "tokens/tokenizer.hpp"
#include "parser/parser.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "USAGE: pangolin <filepath>" << '\n';
		return 1;
	}

	std::ifstream file { argv[1] };
	std::string data {
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	};
	file.close();

	Tokenizer tokenizer(data);
	std::vector<Token> tokens = tokenizer.tokenize();

	// print out tokens
	for (auto t : tokens)
		std::cout << "Token: " << static_cast<int>(t.type) << "; Literal: " << t.literal << "; row: " << t.row << "; col: " << t.col << ";\n";
	std::cout << "\n";

	Parser p(tokens);
	std::unique_ptr<ASTNode> ast = p.parse_program();

	// print out ast
	ast->pretty_print();

	return 0;
}

/**
 * I have replaced the main file for now, so we can interact with our code in the cli
 */
// int main() {
//     std::string input;
//     
//     std::cout << "Simple Expression Calculator" << std::endl;
//     std::cout << "Enter expressions (or 'quit' to exit):" << std::endl;
//     
//     while (true) {
//         std::cout << "> ";
//         std::getline(std::cin, input);
//         
//         if (input == "quit" || input == "exit") {
//             break;
//         }
//         
//         if (input.empty()) {
//             continue;
//         }
//         
//         try {
//             Tokenizer tokenizer(input);
//             auto tokens = tokenizer.tokenize();
//             
//             Parser parser(tokens);
//             auto ast = parser.parse();
//             
//             if (ast) {
//                 double result = ast->evaluate();
//                 std::cout << "Result: " << result << std::endl;
//             }
//         } catch (const std::exception& e) {
//             std::cerr << "Error: " << e.what() << std::endl;
//         }
//     }
//     
//     std::cout << "Goodbye!" << std::endl;
//     return 0;
// }
