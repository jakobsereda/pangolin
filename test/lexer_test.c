#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/lexer.h"

void printToken(Token* token) {
    printf("Token{type: %d, lexeme: '%s', line: %d, column: %d}\n",
           token->type, token->lexeme, token->line, token->column);
}

void testSingleToken(const char* input, TokenType expectedType, const char* expectedLexeme) {
    Lexer lexer;
    init_lexer(&lexer, input);
    Token* token = scanToken(&lexer);
    
    if (token->type != expectedType || strcmp(token->lexeme, expectedLexeme) != 0) {
        printf("Test failed for input '%s':\n", input);
        printf("Expected: {type: %d, lexeme: '%s'}\n", expectedType, expectedLexeme);
        printf("Got: ");
        printToken(token);
        assert(0);
    }
    
    free(token->lexeme);
    free(token);
}

void testKeywords() {
    printf("Testing keywords...\n");
    testSingleToken("let", TOKEN_LET, "let");
    testSingleToken("def", TOKEN_DEF, "def");
    testSingleToken("perchance", TOKEN_PERCHANCE, "perchance");
    testSingleToken("whirl", TOKEN_WHIRL, "whirl");
    testSingleToken("true", TOKEN_TRUE, "true");
    testSingleToken("false", TOKEN_FALSE, "false");
}

void testOperators() {
    printf("Testing operators...\n");
    testSingleToken("+", TOKEN_PLUS, "+");
    testSingleToken("-", TOKEN_MINUS, "-");
    testSingleToken("*", TOKEN_MULTIPLY, "*");
    testSingleToken("/", TOKEN_DIVIDE, "/");
    testSingleToken("++", TOKEN_INCREMENT, "++");
    testSingleToken("--", TOKEN_DECREMENT, "--");
    testSingleToken("<<", TOKEN_LEFTSHIFT, "<<");
    testSingleToken(">>", TOKEN_RIGHTSHIFT, ">>");
    testSingleToken(".&", TOKEN_DOTAMPERSAND, ".&");
    testSingleToken(".|", TOKEN_DOTPIPE, ".|");
}

void testAssignments() {
    printf("Testing assignments...\n");
    testSingleToken("=", TOKEN_EQ, "=");
    testSingleToken("+=", TOKEN_PLUSEQUALS, "+=");
    testSingleToken("-=", TOKEN_MINUSEQUALS, "-=");
    testSingleToken("*=", TOKEN_MULTIPLYEQUALS, "*=");
    testSingleToken("/=", TOKEN_DIVIDEEQUALS, "/=");
    testSingleToken(">>=", TOKEN_RIGHTSHIFTEQUALS, ">>=");
    testSingleToken("<<=", TOKEN_LEFTSHIFTEQUALS, "<<=");
}

void testComparisons() {
    printf("Testing comparisons...\n");
    testSingleToken("==", TOKEN_EE, "==");
    testSingleToken(">=", TOKEN_GEQ, ">=");
    testSingleToken("<=", TOKEN_LEQ, "<=");
    testSingleToken(">", TOKEN_GT, ">");
    testSingleToken("<", TOKEN_LT, "<");
}

void testIdentifiersAndNumbers() {
    printf("Testing identifiers and numbers...\n");
    testSingleToken("variable123", TOKEN_IDENTIFIER, "variable123");
    testSingleToken("123", TOKEN_NUMBER, "123");
    testSingleToken("42", TOKEN_NUMBER, "42");
}

void testWhitespaceAndComments() {
    printf("Testing whitespace and comments...\n");
    Lexer lexer;
    
    // Test whitespace handling
    init_lexer(&lexer, "   let   ");
    Token* token = scanToken(&lexer);
    assert(token->type == TOKEN_LET);
    free(token->lexeme);
    free(token);
    
    // Test comment handling
    init_lexer(&lexer, "let || this is a comment\n x");
    token = scanToken(&lexer);
    assert(token->type == TOKEN_LET);
    free(token->lexeme);
    free(token);
}

void testMultipleTokens() {
    printf("Testing multiple tokens in sequence...\n");
    Lexer lexer;
    init_lexer(&lexer, "let x = 42;");
    
    Token* tokens[5];
    TokenType expectedTypes[] = {
        TOKEN_LET,
        TOKEN_IDENTIFIER,
        TOKEN_EQ,
        TOKEN_NUMBER,
        TOKEN_SEMICOLON
    };
    
    for (int i = 0; i < 5; i++) {
        tokens[i] = scanToken(&lexer);
        assert(tokens[i]->type == expectedTypes[i]);
        free(tokens[i]->lexeme);
        free(tokens[i]);
    }
}

int main() {
    printf("Starting lexer tests...\n\n");
    
    testKeywords();
    testOperators();
    testAssignments();
    testComparisons();
    testIdentifiersAndNumbers();
    testWhitespaceAndComments();
    testMultipleTokens();
    
    printf("\nAll tests passed successfully!\n");
    return 0;
}