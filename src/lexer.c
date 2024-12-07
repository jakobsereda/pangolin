#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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

    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_ERROR,
    TOKEN_EOF               //End of file
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
    int column;
} Lexer;

//Keywords
typedef struct{
    const char* keyword;
    TokenType type;
} Keyword;

//Array of keywords
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

//Initalize the lexer
void init_lexer (Lexer* lexer, const char* source){
    lexer -> source = source;
    lexer -> start = source;
    lexer -> current = source;
    lexer -> line = 1;
    lexer -> column = 1;
}

/**
 * Checks if the current pointer in the lexer points to the end of the source.
 */
static bool atEnd (Lexer* lexer){
    return *lexer -> current == '\0'; //Last character is the null character
}

/**
 * Return the character the current pointer is pointing at in the lexer.
 */
static char peek (Lexer* lexer){
    return *lexer -> current;
}

/**
 * Returns the next character the current pointer is pointing at in the lexer.
 */
static char nextPeek (Lexer* lexer){
    if(atEnd(lexer)) return '\0';
    return *(lexer -> current + 1);
}

/**
 * Checks if expected is the same as the character the current pointer is pointing to in the lexer.
 */
static bool match (Lexer* lexer, char expected){
    if(atEnd(lexer)) return false;
    if(*lexer -> current != expected) return false;

    lexer -> column++;
    lexer -> current++;
    return true;
}

/**
 * Creates a token from the current lexer.
 * 
 * Ryan notes: learn more about malloc and memory
 */
static Token* makeToken(Lexer* lexer, TokenType type) {
    Token* token = (Token*)malloc(sizeof(Token));
    int length = (int)(lexer -> current - lexer -> start);
    token -> lexeme = (char*)malloc(length + 1);
    memcpy(token -> lexeme, lexer -> start, length);
    token -> lexeme[length] = '\0';
    token -> type = type;
    token -> line = lexer -> line;
    token -> column = lexer -> column - length;
    return token;
}

static Token* errorToken(Lexer* lexer, const char* message) {
    Token* token = (Token*)malloc(sizeof(Token));
    token -> type = TOKEN_ERROR;
    token -> lexeme = strdup(message);
    token -> line = lexer -> line;
    token -> column = lexer -> column;
    return token;
}

/**
 * Allows the lexer to ignore whitespace and comments.
 */
static void ignoreWhitespace(Lexer* lexer){
    while(true){
        char c = peek(lexer);
        switch(c){
            case ' ': 
            case '\r':
            case '\t':
                lexer -> column++;
                break;
            case '\n':
                lexer -> line++;
                lexer -> column = 1;
                lexer -> column++;
                break;
            case '|':
                if(nextPeek(lexer) == '|')
                    while(peek(lexer) != '\n' && !atEnd(lexer))
                        lexer -> column++;
                else
                    return;
                break;
            default:
            return;
        }
    }
}

/**
 * Checks if the current token is a keyword.
 */
static TokenType checkKeyword(Lexer* lexer, int start, int length, const char* rest, TokenType type){
	if((int) (lexer -> current - lexer -> start) == start + length && memcmp(lexer -> start + start, rest, length) == 0){
		return type;
	}
	return TOKEN_IDENTIFIER;
}

/**
 * Checks the type of identiifer that a token is.
 */
static TokenType checkIdentifierType (Lexer* lexer){
  int length = lexer -> current - lexer -> start;
  char* text = (char*)malloc(length + 1);
  memcpy(text, lexer -> start, length);
    text[length] = '\0';
    
    for (int i = 0; keywords[i].keyword != NULL; i++){
        if (strcmp(text, keywords[i].keyword) == 0){
            free(text);
            return keywords[i].type;
        }
    }
    
    free(text);
    return TOKEN_IDENTIFIER;
}

static Token* identifier(Lexer *lexer){
  while(isalnum(peek(lexer))){
    lexer -> column++;
  }
  return makeToken(lexer, checkIdentifierType(lexer));
}

static Token* number(Lexer* lexer){
  while(isdigit(peek(lexer))){
    lexer -> column++;
  }
  return makeToken(lexer, TOKEN_NUMBER);
}

Token* scanToken(Lexer* lexer){
  ignoreWhitespace(lexer);
  lexer -> start = lexer -> current;
  
  if(atEnd(lexer)) return makeToken(lexer, TOKEN_EOF);

  lexer -> column++;
  char c = *lexer -> current++;

  if(isalpha(c)) return identifier(lexer);
  if(isdigit(c)) return number(lexer);

  switch(c){
    case('('): return makeToken(lexer, TOKEN_LEFTBRACKET);
    case(')'): return makeToken(lexer, TOKEN_RIGHTBRACKET);
    case('{'): return makeToken(lexer, TOKEN_LEFTCURLY);
    case('}'): return makeToken(lexer, TOKEN_RIGHTCURLY);
    case(','): return makeToken(lexer, TOKEN_COMMA);
    case('.'):
      if(match(lexer, '&')) return makeToken(lexer, TOKEN_DOTAMPERSAND);
      else if(match(lexer, '|')) return makeToken(lexer, TOKEN_DOTPIPE);
      else return makeToken(lexer, TOKEN_DOT);
    case(':'): return makeToken(lexer, TOKEN_COLON);
    case(';'): return makeToken(lexer, TOKEN_SEMICOLON);
  
    case('='): return makeToken(lexer, match(lexer, '=') ? TOKEN_EE : TOKEN_EQ);
    case('<'):
      if(match(lexer, '=')) return makeToken(lexer, TOKEN_LEQ);
      else if(match(lexer, '<')) 
        return makeToken(lexer, match(lexer, '=') ? TOKEN_LEFTSHIFTEQUALS : TOKEN_LEFTSHIFT);
      else return makeToken(lexer, TOKEN_LT);
    case('>'):
      if(match(lexer, '=')) return makeToken(lexer, TOKEN_GEQ);
      else if(match(lexer, '>')) 
        return makeToken(lexer, match(lexer, '=') ? TOKEN_RIGHTSHIFTEQUALS : TOKEN_RIGHTSHIFT);
      else return makeToken(lexer, TOKEN_GT);
    case('~'):
      if (match(lexer, '=')) return makeToken(lexer, TOKEN_TILDEEQUALS);
      if (match(lexer, '&')) return makeToken(lexer, TOKEN_TILDEAMPERSAND);
      if (match(lexer, '|')) return makeToken(lexer, TOKEN_TILDEPIPE);
          return makeToken(lexer, TOKEN_TILDE);
    case '+':
      if (match(lexer, '=')) return makeToken(lexer, TOKEN_PLUSEQUALS);
      if (match(lexer, '+')) return makeToken(lexer, TOKEN_INCREMENT);
      return makeToken(lexer, TOKEN_PLUS);
    case '-':
      if (match(lexer, '=')) return makeToken(lexer, TOKEN_MINUSEQUALS);
      if (match(lexer, '-')) return makeToken(lexer, TOKEN_DECREMENT);
      return makeToken(lexer, TOKEN_MINUS);
    case '*':
      return makeToken(lexer, match(lexer, '=') ? TOKEN_MULTIPLYEQUALS : TOKEN_MULTIPLY); 
    case '/':
      return makeToken(lexer, match(lexer, '=') ? TOKEN_DIVIDEEQUALS : TOKEN_DIVIDE);
    case '%':
      return makeToken(lexer, match(lexer, '=') ? TOKEN_MODEQUALS : TOKEN_MOD);
    case '&':
      return makeToken(lexer, match(lexer, '=') ? TOKEN_AMPERSANDEQUALS : TOKEN_AMPERSAND);
    case '|':
      return makeToken(lexer, match(lexer, '=') ? TOKEN_XOREQUALS : TOKEN_XOR);
  }
  return errorToken(lexer, "Unexpected token");
}
