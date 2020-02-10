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
#include "parser.h"

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
const struct LexToken tokList[] = {
	[TOK_UNKNOWN] = 		{"UNKNOWN",			BOLD_FONT RED_FOREGROUND,			NULL,					NULL},
	[TOK_SPACE] = 			{"SPACE",				WHITE_FOREGROUND, 						isWhiteSpace,	NULL},
	[TOK_COMMENT] = 		{"COMMENT",			CYAN_FOREGROUND, 							isComment,		NULL},
	[TOK_STRING] = 			{"STRING",			GREEN_FOREGROUND, 						isString,			NULL},
	[TOK_NUMBER] = 			{"NUMBER",			YELLOW_FOREGROUND, 						isNumber,			NULL},
	[TOK_KEYWORD] = 		{"KEYWORD",			BOLD_FONT BLUE_FOREGROUND, 		isKeyword,		NULL},
	[TOK_IDENTIFIER] = 	{"IDENTIFIER",	BOLD_FONT MAGENTA_FOREGROUND, isIdentifier,	NULL}, /* Variable or function name */
	[TOK_DELIMITER] = 	{"DELIMITER",		WHITE_FOREGROUND, 						isDelimiter,	NULL},
	[TOK_COUNT] = 			{0}
};

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
	0
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

static void increaseCodePosition(struct Positioning *pos, size_t length) {
	while (length--) {
		pos->col++;
		if (*pos->string == '\n') {
			pos->line++;
			pos->col = 1;
		}
		pos->string++;
	}
}

static bool isWhiteSpace(struct Token *const token) {
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

static bool isComment(struct Token *const token) {
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

static bool isString(struct Token *const token) {
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

static bool isNumber(struct Token *const token) {
	char *numberEnd = (char *) token->pos.string;
	token->subToken.number = strtod(token->pos.string, &numberEnd);
	if (numberEnd != token->pos.string) {
		token->pos.length = numberEnd - token->pos.string;
		return true;
	}
	return false;
}

static bool isKeyword(struct Token *const token) {
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
	// Isn't a keyword
	return false;
}

static bool isIdentifier(struct Token *const token) {
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

static bool isDelimiter(struct Token *const token) {
	// Sequential search in the list
	for (int index = 0; delimiterList[index]; index++) {
		size_t length = strlen(delimiterList[index]);
		if (!strncmp(token->pos.string, delimiterList[index], length)) {
			token->pos.length = length;
			token->subToken.delimiter = index;
			return true;
		}
	}
	return false;
}

static inline size_t max(const size_t x, const size_t y) {
	return (((x) > (y)) ? (x) : (y));
}

static bool realocateTokenList(struct TokenList *tokens, size_t size) {
	if (size) {
		struct Token *newList = (struct Token *) realloc(tokens->list, size*sizeof(struct Token));
		if (!newList) {
			free((void*)tokens->list);
			tokens->list = NULL;
			tokens->size = 0;
			return false;
		}
		tokens->list = newList;
		tokens->size = size;
	}
	return true;
}

struct TokenList *lex(const char *const fileContends) {
	unsigned int tkIndex;
	struct Positioning pos = {
		.string = fileContends, .length = 0, .line = 1, .col = 1,
	};
	struct TokenList *tokens = (struct TokenList *) malloc(sizeof(struct TokenList));
	tokens->list = NULL;
	tokens->size = 0;

	for (tkIndex = 0; *pos.string; tkIndex++) {

		// Allocates more memory if necessary
		if (tkIndex >= tokens->size) {
			if (!realocateTokenList(tokens, 2 * max(tokens->size, 1))) {
				printCrash("Lexical analysis error.\n");
				break;
			}
		}
		// Initializes next token
		tokens->list[tkIndex] = (struct Token) {
			.type = TOK_UNKNOWN, .pos = pos,
		};
		// Sequential search to find type of next token
		for (int checkIdx = TOK_SPACE; tokList[checkIdx].name; checkIdx++) {
			// If found the token type
			if (tokList[checkIdx].check(&tokens->list[tkIndex])) {
				tokens->list[tkIndex].type = checkIdx;
				increaseCodePosition(&pos, tokens->list[tkIndex].pos.length);
				break;
			}
		}

		// Stop at the first error found
		if (errorsInTheCode()) {
			break;
		}
		// If the token is still unknown, break the loop
		if (tokens->list[tkIndex].type == TOK_UNKNOWN) {
			printError(tokens->list[tkIndex].pos, "Ilegal definition.\n");
			break;
		}
	}
	// Free unused memory
	realocateTokenList(tokens, tkIndex);
	// Return the list of tokens
	return tokens;
}

static inline void printLineString(const char *string, const int length) {
	static const int maxLenString = 20;

	for (int i = 0; i < length; i++, string++) {
		if ((*string == '\n') || (i >= maxLenString)) {
			printf("...");
			break;
		}
		putchar(*string);
	}
}

void printTokenList(struct TokenList *tokens)
{
	printf("Line\tColumn\tToken Type\tValue\n");
	for (unsigned int tkIndex = 0; tkIndex < tokens->size; tkIndex++) {
		// Doesn't print spaces and unknown tokens
		if (tokens->list[tkIndex].type > TOK_SPACE) {
			printf("%s", tokList[tokens->list[tkIndex].type].font);
			printf("%u\t", tokens->list[tkIndex].pos.line);
			printf("%u\t", tokens->list[tkIndex].pos.col);
			printf("%-15s\t", tokList[tokens->list[tkIndex].type].name);
			printLineString(tokens->list[tkIndex].pos.string, tokens->list[tkIndex].pos.length);
			puts(RESET_FONT);
		}
	}
}

void printColoredCode(const char *code, struct TokenList *tokens)
{
	unsigned int tkIndex = 0;
	for (; *code != '\0'; code++) {
		if (code >= tokens->list[tkIndex].pos.string) {
			printf(RESET_FONT "%s", tokList[tokens->list[tkIndex].type].font);
			tkIndex++;
			if (tkIndex >= tokens->size) {
				tkIndex = tokens->size - 1;
			}
		}
		putchar(*code);
	}
	puts(RESET_FONT);
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
