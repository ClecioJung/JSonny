//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "definitions.h"
#include "lex.h"
#include "errors.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

#define IS_NEW_LINE(X)			((X) == '\n')
#define IS_SPACE(X)					(strchr(" \t\n\r", (X)))
#define IS_NUMBER(X)				(isdigit((X)))
#define IS_FLOAT_NUMBER(X)	(isdigit((X)) || ((X) == '.'))

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

struct Token {
	int type;
	const char *string;
	int length;
	unsigned int line;
	unsigned int col;
	int subToken;
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

static bool isWhiteSpace(struct Token *const token);
static bool isComment(struct Token *const token);
static bool isString(struct Token *const token);
static bool isNumber(struct Token *const token);
static bool isKeyword(struct Token *const token);
static bool isIdentifier(struct Token *const token);
static bool isDelimiter(struct Token *const token);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

// List of token types
static const struct LexToken tokList[] = {
	{"UNKNOWN",			BOLD_FONT RED_FOREGROUND,			true,		NULL},
	{"SPACE",				WHITE_FOREGROUND, 						false, 	isWhiteSpace},
	{"COMMENT",			CYAN_FOREGROUND, 							true,		isComment},
	{"STRING",			GREEN_FOREGROUND, 						true,		isString},
	{"NUMBER",			YELLOW_FOREGROUND, 						true,		isNumber},
	{"KEYWORD",			BOLD_FONT BLUE_FOREGROUND, 		true,		isKeyword},
	{"IDENTIFIER",	BOLD_FONT MAGENTA_FOREGROUND, true,		isIdentifier}, /* Variable or function name */
	{"DELIMITER",		WHITE_FOREGROUND, 						true,		isDelimiter},
};
const int tokCount = sizeof(tokList)/sizeof(tokList[0]);

//  List of interpretable keywords
const char *const keywordList[] = {
	"break",
	"case", "catch", "const", "continue",
	"default", "delete", "do",
	"else",
	"false", "final", "finally", "for", "function",
	"if", "in",	"instanceof",
	"let",
	"new",
	"return",
	"switch",
	"this", "throw", "true", "try", "typeof",
	"var",
	"while",
};
const int keywordCount = sizeof(keywordList)/sizeof(keywordList[0]);

//  List of interpretable delimiters
// The order is relevant, example: ++ must come before +
const char *const delimiterList[] = {
	"++", "--", "+", "-", "*", "/", "%%",
	"!==", "!=", "===", "==", "+=", "-=", "*=", "/=", "%%=",
	">=", "<=", "=", ">>>", "<<", ">>", ">", "<",
	"&&", "||", "!", "&", "|", "~", "^",
	"?", ":",
	"{", "}", "(", ")",
	";", ",", ".",
};
const int delimCount = sizeof(delimiterList)/sizeof(delimiterList[0]);

struct TokenList tokens = {
	.list = NULL,
	.size = 0
};

const char *code = NULL;
unsigned int line = 1, col = 1;
unsigned int errors = 0;
unsigned int warnings = 0;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

static void printError(const char *const msg, ...)
{
	va_list args;
	const char *const startLine = code - col + 1;
	const unsigned int lineLength = strchr(startLine, '\n') - startLine;

	// Print header in stream
	fprintf(stderr, ERROR_HEADER "in line %u column %u: ", line, col);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);

	// Print code line to indicate where the error is
	fprintf(stderr, "%.*s\n", lineLength, startLine);
	for (unsigned int columns = col; columns > 1; columns--) {
		putc(' ', stderr);
	}
	fprintf(stderr, "^\n");

	// Increase the error counter
	errors++;
}

static void printWarning(const char *const msg, ...)
{
	va_list args;
	const char *const startLine = code - col + 1;
	const unsigned int lineLength = strchr(startLine, '\n') - startLine;

	// Print header in stream
	fprintf(stderr, WARNING_HEADER "in line %u column %u: ", line, col);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);

	// Print code line to indicate where the error is
	fprintf(stderr, "%.*s\n", lineLength, startLine);
	for (unsigned int columns = col; columns > 1; columns--) {
		putc(' ', stderr);
	}
	fprintf(stderr, "^\n");

	// Increase the warning counter
	warnings++;
}

// Go to next character
static void nextChar(void)
{
	col++;
	if (IS_NEW_LINE(*code)) {
		line++;
		col = 1;
	}
	code++;
}

// Increase char position
static void increaseChar(size_t length)
{
	col += length;
	code += length;
}

static bool isWhiteSpace(struct Token *const token)
{
	if (IS_SPACE(*code)) {
		do {
			nextChar();
		} while ((*code) && IS_SPACE(*code));

		token->length = code - token->string;
		return true;
	}
	return false;
}

static bool isComment(struct Token *const token)
{
	// Multiline comment
	if (!strncmp(code, "/*", 2)) {
		increaseChar(1);
		do {
			nextChar();
			// If the end of file is found in midlle of a token
			if (!*code) {
				printError("Incomplete statement.\n");
				break;
			}
		} while (strncmp(code, "*/", 2));
		increaseChar(2);

		token->length = code - token->string;
		return true;

		// Single line comment
	} else if (!strncmp(code, "//", 2)) {
		increaseChar(1);
		do {
			nextChar();
			// If the end of file is found in midlle of a token
			if (!*code) {
				printError("Incomplete statement.\n");
				break;
			}
		} while ((*code) && ((*code != '\n') || (*(code-1) == '\\')));

		token->length = code - token->string;
		return true;
	}
	return false;
}

static bool isString(struct Token *const token)
{
	if (*code == '"' || *code == '\'') {
		const char initString = *code;
		do {
			nextChar();
			// If the end of file is found in midlle of a token
			if (!*code) {
				printError("Incomplete statement.\n");
				break;
				// If a new line is found in the middle of definition
			} else if ((*code == '\n') && (*(code-1) != '\\')) {
				printError("String not properly defined.\n");
				break;
			}
		} while (*code != initString);
		nextChar();

		token->length = code - token->string;
		return true;
	}
	return false;
}

static bool isNumber(struct Token *const token)
{
	if (IS_NUMBER(*code)) {
		do {
			nextChar();
		} while ((*code) && IS_FLOAT_NUMBER(*code));
		token->length = code - token->string;
		return true;
	}
	return false;
}

static bool isKeyword(struct Token *const token)
{
	// Sequential search in the list
	for (int index = 0; index < keywordCount; index++) {
		size_t length = strlen(keywordList[index]);
		if (!strncmp(code, keywordList[index], length)) {
			increaseChar(length);
			token->length = length;
			token->subToken = index;
			return true;
		}
	}
	return false;
}

static bool isIdentifier(struct Token *const token)
{
	if ((isalpha(*code)) || (*code == '_')) {
		do {
			nextChar();
		} while ((isalnum(*code)) || (*code == '_') || IS_NUMBER(*code));
		token->length = code - token->string;
		return true;
	}
	return false;
}

static bool isDelimiter(struct Token *const token)
{
	// Sequential search in the list
	for (int index = 0; index < delimCount; index++) {
		size_t length = strlen(delimiterList[index]);
		if (!strncmp(code, delimiterList[index], length)) {
			increaseChar(length);
			token->length = length;
			token->subToken = index;
			return true;
		}
	}
	return false;
}

static inline size_t max(const size_t x, const size_t y)
{
	return (((x) > (y)) ? (x) : (y));
}

static inline bool realocateTokenList(void)
{
	if (tokens.lastIdx >= tokens.size) {
		tokens.size = 2 * max(tokens.size, 1);
		struct Token *newList = (struct Token *) realloc(tokens.list, tokens.size*sizeof(struct Token));
		if (!newList) {
			free(tokens.list);
			tokens.list = NULL;
			tokens.size = 0;
			return false;
		}
		tokens.list = newList;
	}
	return true;
}

void lex(const char *const prog)
{
	code = prog;
	for (tokens.lastIdx = 0; *code; tokens.lastIdx++) {
		// Allocates more memory if necessary
		if (!realocateTokenList()) {
			printCrash("Lexical analysis error.\n");
			break;
		}
		// Initializes next token
		tokens.list[tokens.lastIdx] = (struct Token) {
			.type = 0,
			.string = code, .length = 0,
			.line = line, .col = col,
			.subToken = -1,
		};
		// Sequential search to find type of next token
		for (int checkIdx = 1; checkIdx < tokCount; checkIdx++) {
			// If found the token type
			if (tokList[checkIdx].check(&tokens.list[tokens.lastIdx])) {
				tokens.list[tokens.lastIdx].type = checkIdx;
				break;
			}
		}
		// Stop at the first error found
		if (errors) {
			break;
		}
		// If the token is still unknown, break the loop
		if (tokens.list[tokens.lastIdx].type == -1) {
			printError("Ilegal definition.\n");
			break;
		}
	}
}

void freeLex(void)
{
	free((void*)tokens.list);
	tokens.list = NULL;
}

static inline void printLineString(const char *string, const int length)
{
	static const int maxLenString = 20;

	for (int i = 0; i < length; i++, string++) {
		if ((*string == '\n') || (i >= maxLenString)) {
			printf("...");
			break;
		}
		putchar(*string);
	}
}

void printTokenList(void)
{
	printf("Line\tColumn\tToken Type\tValue\n");
	for (unsigned int tkIndex = 0; tkIndex < tokens.lastIdx; tkIndex++) {
		if (tokList[tokens.list[tkIndex].type].print) {
			fputs(tokList[tokens.list[tkIndex].type].font, stdout);
			printf("%u\t", tokens.list[tkIndex].line);
			printf("%u\t", tokens.list[tkIndex].col);
			printf("%-15s\t", tokList[tokens.list[tkIndex].type].name);
			printLineString(tokens.list[tkIndex].string, tokens.list[tkIndex].length);
			puts(RESET_FONT);
		}
	}
}

void printColoredCode(const char *code)
{
	for (unsigned int tkIndex = 0; *code != '\0'; code++) {
		if (code >= tokens.list[tkIndex].string) {
			printf(RESET_FONT "%s", tokList[tokens.list[tkIndex].type].font);
			tkIndex++;
			if (tkIndex > tokens.lastIdx) {
				tkIndex = tokens.lastIdx;
			}
		}
		putchar(*code);
	}
	printf(RESET_FONT);
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
