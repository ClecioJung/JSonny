#ifndef __LEX
#define __LEX

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include "definitions.h"
#include "jsonny.h"

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

/*  List of token types
- Example:
X(TOKEN_NAME,	COLOR_DEFINITION)
*/
#define X_TOKENS(X) \
X(UNKNOWN,		BOLD_RED ) \
X(SPACE,		NO_COLOR ) \
X(IDENTIFIER,	BOLD_MAGENTA ) /* Variable or function name */ \
X(DELIMITER,	NO_COLOR ) \
X(KEYWORD,		BOLD_BLUE ) \
X(NUMBER,		NORMAL_YELLOW ) \
X(STRING,		NORMAL_CYAN ) \
X(COMMENT,		NORMAL_GREEN )

// Auxiliar definitions for tokens X MACRO
#define X_TOK_EXPAND_ENUM(a,b)			a,
#define X_TOK_EXPAND_TABLE(a,b)			{.name = b #a NO_COLOR, .color = b},
#define X_TOK_EXPAND_COUNT(a,b)			+1
#define TOK_COUNT						(0 X_TOKENS(X_TOK_EXPAND_COUNT))

/*  List of functions to test token types
- Particular cases must be treated in lex() function
- This functions must be defined in lex.c before lex(), and isn't necessary to
create their prototypes
- Error condition can be defined as NULL
- Example:
X(TOKEN_NAME,	beginCondition,			endCondition,			errorCondition )
*/
#define X_TEST_TOK(X) \
X(SPACE,		beginWhiteSpace,		endWhiteSpace,			NULL ) \
X(COMMENT,		beginMultiLineComment,	endMultiLineComment,	NULL ) \
X(COMMENT,		beginLineComment,		endLineComment,			NULL ) \
X(DELIMITER,	beginDelimiter,			endDelimiter,			NULL ) \
X(NUMBER,		beginNumber,			endNumber,				NULL ) \
X(STRING,		isString,				isString,				testString ) \
X(STRING,		isApostropheString,		isApostropheString,		testString ) \
X(IDENTIFIER,	beginIdentifier,		endIdentifier,			NULL )

// Auxiliar definitions for test conditions X MACRO
#define X_TEST_EXPAND_TABLE(a,b,c,d)	{.type = a, .begin = b, .end = c, .error = d},
#define X_TEST_EXPAND_COUNT(a,b,c,d)	+1
#define TEST_COUNT						(0 X_TEST_TOK(X_TEST_EXPAND_COUNT))

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum TokenType {
	X_TOKENS(X_TOK_EXPAND_ENUM)
};

struct Token {
	enum TokenType type;
	char *string;
	int length;
	unsigned int line;
	unsigned int col;
};

struct TokenList {
	struct Token *list;
	size_t lastIndex;
	size_t size;
};

// User type used to store token types
struct LexToken {
	char *name;
	char *color;
};

typedef int (*TestStringForToken)(const char *const);
typedef char *(*TestErrorString)(const char *const);

// User type used to store token conditions
struct TokenCondition {
	enum TokenType type;
	TestStringForToken begin;
	TestStringForToken end;
	TestErrorString error;
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

void printTokenList(const struct TokenList *const tokens);
void printColoredCode(const char *code, const struct TokenList *const tokens);
struct TokenList lex(struct EnvironmentData *const env, const char *code);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __LEX
