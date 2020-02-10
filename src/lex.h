#ifndef __LEX
#define __LEX

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdbool.h>
#include "definitions.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum TokTypes {
	TOK_UNKNOWN = 0,
	TOK_SPACE,
	TOK_COMMENT,
	TOK_STRING,
	TOK_NUMBER,
	TOK_KEYWORD,
	TOK_IDENTIFIER,
	TOK_DELIMITER,

	TOK_COUNT
};

struct Positioning {
	const char *string;
	unsigned int length;
	unsigned int line;
	unsigned int col;
};

struct Token {
	int type;
	struct Positioning pos;
	union SubToken {
		int delimiter;
		int keyword;
		double number;
	} subToken;
};

// Table used to save the list of tokens
struct TokenList {
	struct Token *list;
	size_t size;
};

// User type used to store token conditions
typedef bool (*CheckForToken)(struct Token *const);

// User type used to store functions to parse each token type
typedef void (*ParseToken)(const struct TokenList *const, unsigned int);

// User type used to store token types
struct LexToken {
	const char *name;
	const char *font;
	CheckForToken check;
	ParseToken parse;
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

struct TokenList *lex(const char *const prog);
void printTokenList(struct TokenList *tokens);
void printColoredCode(const char *code, struct TokenList *tokens);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

extern const struct LexToken tokList[];

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __LEX
