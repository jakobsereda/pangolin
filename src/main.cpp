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

    Tokenizer tokenizer(data);
    std::vector<Token> tokens = tokenizer.tokenize();
    
    for (auto t : tokens)
		std::cout << "Token: " << static_cast<int>(t.type) << "; Literal: " << t.literal << "; row: " << t.row << "; col: " << t.col << ";\n";
    
    return 0;
}

/**
 * I have replaced the main file for now, so we can interact with our code in the cli
 *
 * I'm commenting this out but you can feel free to uncomment it if you want to use it
 * to test. Really we should probably write unit tests before we write any code because
 * eventually things will get too big for cli testing like this (unless we are writing an
 * interpreter instead of a compiler, but regardless tests are good).
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
