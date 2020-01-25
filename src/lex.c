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

#define USE_BINARY_SEARCH_FOR_KEYWORDS 1

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

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

// List of interpretable keywords
// Insert in alphabetic order, so we can use binary search
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

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

static void increaseCodePosition(struct Positioning *pos, size_t length)
{
	while (length--) {
		pos->col++;
		if (*pos->string == '\n') {
			pos->line++;
			pos->col = 1;
		}
		pos->string++;
	}
}

static bool isWhiteSpace(struct Token *const token)
{
	const char *string = token->pos.string;
	while (isspace(*string)) {
		string++;
	}
	if (string != token->pos.string) {
		token->pos.length = string - token->pos.string;
		return true;
	}
	return false;
}

static bool isComment(struct Token *const token)
{
	const char *string = token->pos.string;
	// Multiline comment
	if (!strncmp(string, "/*", 2)) {
		string++;
		do {
			string++;
			// If the end of file is found in midlle of a token
			if (!*string) {
				printError(token->pos, "Incomplete statement.\n");
				break;
			}
		} while (strncmp(string, "*/", 2));
		string += 2;

		token->pos.length = string - token->pos.string;
		return true;

		// Single line comment
	} else if (!strncmp(string, "//", 2)) {
		string++;
		do {
			string++;
			// If the end of file is found in midlle of a token
			if (!*string) {
				printError(token->pos, "Incomplete statement.\n");
				break;
			}
		} while ((*string) && ((*string != '\n') || (*(string-1) == '\\')));

		token->pos.length = string - token->pos.string;
		return true;
	}
	return false;
}

static bool isString(struct Token *const token)
{
	const char *string = token->pos.string;
	if (*string == '"' || *string == '\'') {
		const char initString = *string;
		do {
			string++;
			// If the end of file is found in midlle of a token
			if (!*string) {
				printError(token->pos, "Incomplete statement.\n");
				break;
				// If a new line is found in the middle of definition
			} else if ((*string == '\n') && (*(string-1) != '\\')) {
				printError(token->pos, "String not properly defined.\n");
				break;
			}
		} while (*string != initString);
		string++;

		token->pos.length = string - token->pos.string;
		return true;
	}
	return false;
}

static bool isNumber(struct Token *const token)
{
	char *numberEnd = (char *) token->pos.string;
	token->subToken.number = strtod(token->pos.string, &numberEnd);
	if (numberEnd != token->pos.string) {
		token->pos.length = numberEnd - token->pos.string;
		return true;
	}
	return false;
}

static bool isKeyword(struct Token *const token)
{
	#if USE_BINARY_SEARCH_FOR_KEYWORDS
	// Binary Search in the list
	// Based on: https://www.tutorialspoint.com/binary-search-a-string-in-cplusplus
	const char *string = token->pos.string;
	int lower = 0;
	int upper = keywordCount - 1;
	while (lower <= upper) {
		int mid = (lower + upper) / 2;
		size_t length = strlen(keywordList[mid]);
		int res = strncmp(string, keywordList[mid], length);
		if (res == 0) {
			// Test if is not a identifier
			if (isalnum(string[length]) || string[length] == '_' || isdigit(string[length])) {
				return false;
			}
			// Found
			token->pos.length = length;
			token->subToken.keyword = mid;
			return true;
		} else if (res > 0) {
			lower = mid + 1;
		} else {
			upper = mid - 1;
		}
	}
	#else
	// Sequential search in the list
	const char *string = token->pos.string;
	for (int index = 0; index < keywordCount; index++) {
		size_t length = strlen(keywordList[index]);
		if (!isalnum(string[length]) && string[length] != '_' && !isdigit(string[length])) {
			if (!strncmp(string, keywordList[index], length)) {
				token->pos.length = length;
				token->subToken.delimiter = index;
				return true;
			}
		}
	}
	#endif
	// Isn't a keyword
	return false;
}

static bool isIdentifier(struct Token *const token)
{
	const char *string = token->pos.string;
	if ((isalpha(*string)) || (*string == '_')) {
		do {
			string++;
		} while (isalnum(*string) || *string == '_' || isdigit(*string));
		token->pos.length = string - token->pos.string;
		return true;
	}
	return false;
}

static bool isDelimiter(struct Token *const token)
{
	// Sequential search in the list
	for (int index = 0; index < delimCount; index++) {
		size_t length = strlen(delimiterList[index]);
		if (!strncmp(token->pos.string, delimiterList[index], length)) {
			token->pos.length = length;
			token->subToken.delimiter = index;
			return true;
		}
	}
	return false;
}

static inline size_t max(const size_t x, const size_t y)
{
	return (((x) > (y)) ? (x) : (y));
}

static bool realocateTokenList(size_t size)
{
	if (size) {
		struct Token *newList = (struct Token *) realloc(tokens.list, size*sizeof(struct Token));
		if (!newList) {
			freeLex();
			return false;
		}
		tokens.list = newList;
		tokens.size = size;
	}
	return true;
}

void lex(const char *const fileContends)
{
	struct Positioning pos = {
		.string = fileContends,
		.length = 0,
		.line = 1,
		.col = 1,
	};
	for (tokens.lastIdx = 0; *pos.string; tokens.lastIdx++) {

		// Allocates more memory if necessary
		if (tokens.lastIdx >= tokens.size) {
			if (!realocateTokenList(2 * max(tokens.size, 1))) {
				printCrash("Lexical analysis error.\n");
				break;
			}
		}
		// Initializes next token
		tokens.list[tokens.lastIdx] = (struct Token) {
			.type = 0,
			.pos = pos,
		};
		// Sequential search to find type of next token
		for (int checkIdx = 1; checkIdx < tokCount; checkIdx++) {
			// If found the token type
			if (tokList[checkIdx].check(&tokens.list[tokens.lastIdx])) {
				tokens.list[tokens.lastIdx].type = checkIdx;
				increaseCodePosition(&pos, tokens.list[tokens.lastIdx].pos.length);
				break;
			}
		}
		// Stop at the first error found
		if (errorsInTheCode()) {
			break;
		}
		// If the token is still unknown, break the loop
		if (!tokens.list[tokens.lastIdx].type) {
			printError(tokens.list[tokens.lastIdx].pos, "Ilegal definition.\n");
			break;
		}
	}
	// Free unused memory
	realocateTokenList(tokens.lastIdx);
}

void freeLex(void)
{
	free((void*)tokens.list);
	tokens.list = NULL;
	tokens.size = 0;
	tokens.lastIdx = 0;
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
			printf("%u\t", tokens.list[tkIndex].pos.line);
			printf("%u\t", tokens.list[tkIndex].pos.col);
			printf("%-15s\t", tokList[tokens.list[tkIndex].type].name);
			printLineString(tokens.list[tkIndex].pos.string, tokens.list[tkIndex].pos.length);
			puts(RESET_FONT);
		}
	}
}

void printColoredCode(const char *code)
{
	unsigned int tkIndex = 0;
	for (; *code != '\0'; code++) {
		if (code >= tokens.list[tkIndex].pos.string) {
			printf(RESET_FONT "%s", tokList[tokens.list[tkIndex].type].font);
			tkIndex++;
			if (tkIndex >= tokens.lastIdx) {
				tkIndex = tokens.lastIdx - 1;
			}
		}
		putchar(*code);
	}
	printf(RESET_FONT);
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
