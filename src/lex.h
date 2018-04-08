#ifndef __LEX
#define __LEX

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include "definitions.h"

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

/*  List of token types
- Example:
X(TOKEN_NAME,	COLOR_DEFINITION)
*/
#define X_TOKENS(X) \
X(UNKNOWN,		BOLD_FONT RED_FOREGROUND ) \
X(SPACE,			WHITE_FOREGROUND ) \
X(IDENTIFIER,	BOLD_FONT MAGENTA_FOREGROUND ) /* Variable or function name */ \
X(DELIMITER,	WHITE_FOREGROUND ) \
X(KEYWORD,		BOLD_FONT BLUE_FOREGROUND ) \
X(NUMBER,		YELLOW_FOREGROUND ) \
X(STRING,		GREEN_FOREGROUND ) \
X(COMMENT,		CYAN_FOREGROUND )

// Auxiliar definitions for tokens X MACRO
#define X_TOK_EXPAND_ENUM(a,b)			a,
#define X_TOK_EXPAND_TABLE(a,b)			{.name = #a, .font = b},
#define X_TOK_EXPAND_COUNT(a,b)			+1
#define TOK_COUNT								(0 X_TOKENS(X_TOK_EXPAND_COUNT))

/*  List of functions to test token types
- Particular cases must be treated in treatParticularTokens() function
- This functions must be defined in lex.c before lex(), and isn't necessary to
create their prototypes
- Error condition can be defined as NULL, which means no test is made
- Example:
X(TOKEN_NAME,	beginCondition,			endCondition,			errorCondition )
*/
#define X_TEST_TOK(X) \
X(SPACE,			beginWhiteSpace,			endWhiteSpace,			NULL ) \
X(COMMENT,		beginMultiLineComment,	endMultiLineComment,	NULL ) \
X(COMMENT,		beginLineComment,			endLineComment,		NULL ) \
X(STRING,		isString,					isString,				testString ) \
X(STRING,		isApostropheString,		isApostropheString,	testString ) \
X(NUMBER,		beginNumber,				endNumber,				NULL ) \
X(IDENTIFIER,	beginIdentifier,			endIdentifier,			NULL )

// Auxiliar definitions for test conditions X MACRO
#define X_TEST_EXPAND_TABLE(a,b,c,d)	{.type = a, .begin = b, .end = c, .error = d},
#define X_TEST_EXPAND_COUNT(a,b,c,d)	+1
#define TEST_COUNT							(0 X_TEST_TOK(X_TEST_EXPAND_COUNT))

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

#define MAX(X,Y)							(((X) > (Y)) ? (X) : (Y))

#define IS_NEW_LINE(X)					((X) == '\n')
#define IS_SPACE(X)						(strchr(" \t\n\r", (X)))
#define IS_NUMBER(X)						(isdigit((X)))
#define IS_FLOAT_NUMBER(X)				(isdigit((X)) || ((X) == '.'))

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum TokenType {
	X_TOKENS(X_TOK_EXPAND_ENUM)
};

struct Token {
	enum TokenType type;
	const char *string;
	int length;
	unsigned int line;
	unsigned int col;
	int subToken;
};

// Table used to save the list of tokens
struct TokenList {
	struct Token *list;
	size_t lastIndex;
	size_t size;
};

// User type used to store token types
struct LexToken {
	char *name;
	char *font;
};

// User type used to store token conditions
typedef int (*TestStringForToken)(const char *const);
typedef char *(*TestErrorString)(const char *const);
struct TokenCondition {
	enum TokenType type;
	TestStringForToken begin;
	TestStringForToken end;
	TestErrorString error;
};

// This struct is defined in jsonny.h, but lex() must know it exist
struct EnvironmentData;

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

void lex(struct EnvironmentData *const env);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __LEX
