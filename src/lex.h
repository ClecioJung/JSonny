#ifndef __LEX
#define __LEX

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------
#include <stdlib.h>

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

struct Positioning {
	const char *string;
	unsigned int length;
	unsigned int line;
	unsigned int col;
};

union SubToken {
	int delimiter;
	int keyword;
	double number;
};

struct Token {
	int type;
	struct Positioning pos;
	union SubToken subToken;
};

// Table used to save the list of tokens
struct TokenList {
	struct Token *list;
	size_t size;
	size_t lastIdx;
};

// User type used to store token conditions
typedef bool (*CheckForToken)(struct Token *const);

// User type used to store token types
struct LexToken {
	const char *name;
	const char *font;
	bool print; // for printTokenList() method
	CheckForToken check;
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

void lex(const char *const prog);
void freeLex(void);
void printTokenList(void);
void printColoredCode(const char *code);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __LEX
