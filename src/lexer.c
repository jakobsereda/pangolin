#include <stdio.h>
#include <stdlib.h>

typedef enum {
    // keywords
    TOKEN_LET,
    TOKEN_DEF,
    TOKEN_PERCHANCE,
    TOKEN_MAYHAPS,
    TOKEN_PERHAPS,
    TOKEN_WHIRL,
    TOKEN_TWIRL,
    TOKEN_IN,
    TOKEN_TRUE,
    TOKEN_FALSE,

    //operators
    TOKEN_TILDE,            //~
    TOKEN_PLUS,             //+
    TOKEN_MINUS,            //-
    TOKEN_MULTIPLY,         //*
    TOKEN_DIVIDE,           // /
    TOKEN_MOD,              //%
    TOKEN_LEFTSHIFT,        //<<
    TOKEN_RIGHTSHIFT,       //>>
    TOKEN_AMPERSAND,        //&
    TOKEN_PIPE,             //|
    TOKEN_XOR,              //^
    TOKEN_DOTAMPERSAND,     //.&
    TOKEN_DOTPIPE,          //.|
    TOKEN_TILDEAMPERSAND,   //~&
    TOKEN_TILDEPIPE,        //~|
    TOKEN_INCREMENT,        //++
    TOKEN_DECREMENT,        //--

    //assignments
    TOKEN_TILDEEQUALS,      //~=
    TOKEN_PLUSEQUALS,       //+=
    TOKEN_MINUSEQUALS,      //-=
    TOKEN_MULTIPLYEQUALS,   //*=
    TOKEN_DIVIDEEQUALS,     // /=
    TOKEN_MODEQUALS,        //%=
    TOKEN_AMPERSANDEQUALS,  //&=
    TOKEN_PIPEEQUALS,       //|=
    TOKEN_XOREQUALS,        //^=
    TOKEN_RIGHTSHIFTEQUALS, //>>=
    TOKEN_LEFTSHIFTEQUALS,  //<<=

    //comparisons
    TOKEN_EE,               //==
    TOKEN_GEQ,              //>=
    TOKEN_LEQ,              //<=
    TOKEN_LT,               //<
    TOKEN_GT,               //>

    //punctuation
    TOKEN_COLON,            //:
    TOKEN_SEMICOLON,        //;
    TOKEN_COMMA,            //,
    TOKEN_DOT,              //.
    TOKEN_LEFTBRACKET,      //(
    TOKEN_RIGHTBRACKET,     //)
    TOKEN_LEFTCURLY,        //{
    TOKEN_RIGHTCURLY,       //}
    TOKEN_EQ,               //=

    TOKEN_NUMBER,
    TOKEN_ERROR
} TokenType;

//Tokenizer
typedef struct {
    TokenType type;
    char* lexeme;
    int line;
    int column;
} Token;

//Lexer
typedef struct{
    const char* source;
    const char* start;
    const char* current;
    int line;
    int colunmn;
} Lexer;

typedef struct{
    const char* keyword;
    TokenType type;
} Keyword;

static Keyword keywords[] = {
    {"let", TOKEN_LET},
    {"def", TOKEN_DEF},
    {"perchance", TOKEN_PERCHANCE},
    {"mayhaps", TOKEN_MAYHAPS},
    {"perhaps", TOKEN_PERHAPS},
    {"whirl", TOKEN_WHIRL},
    {"twirl", TOKEN_TWIRL},
    {"in", TOKEN_IN},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {NULL, TOKEN_ERROR}  // sentinel
};
