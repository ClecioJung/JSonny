//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "keywords.h"
#include "lex.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

#define MAX(X,Y)						(((X) > (Y)) ? (X) : (Y))

#define MAX_STRING						20

#define IS_SPACE(X)						(strchr(" \t\n\r", (X)))
#define IS_DELIMITER(X)					(strchr("+-*/%%!=><&|~^?:{}[]();,.", (X)))
#define IS_NUMBER(X)					(isdigit((X)) || ((X) == '.'))

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static const struct LexToken tokList[TOK_COUNT] = {
	X_TOKENS(X_TOK_EXPAND_TABLE)
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

int beginWhiteSpace(const char *const code) {
	if (IS_SPACE(*code)) {
		return 0;
	}
	return -1;
}
int endWhiteSpace(const char *const code) {
	if (!((*code) && IS_SPACE(*code))) {
		return 0;
	}
	return -1;
}

int beginMultiLineComment(const char *const code) {
	if (!strncmp(code, "/*", 2)) {
		return 2;
	}
	return -1;
}
int endMultiLineComment(const char *const code) {
	if (!strncmp(code, "*/", 2)) {
		return 2;
	}
	return -1;
}

int beginLineComment(const char *const code) {
	if (!strncmp(code, "//", 2)) {
		return 2;
	}
	return -1;
}
int endLineComment(const char *const code) {
	if ((*code == '\n') && (*(code-1) != '\\')) {
		return 0;
	}
	return -1;
}

int isString(const char *const code) {
	if (*code == '"') {
		return 1;
	}
	return -1;
}
int isApostropheString(const char *const code) {
	if (*code == '\'') {
		return 1;
	}
	return -1;
}
char *testString(const char *const code) {
	if ((*code == '\n') && (*(code-1) != '\\')) {
		return "String not properly defined.\n";
	}
	return NULL;
}

int beginDelimiter(const char *const code) {
	if (IS_DELIMITER(*code)) {
		return 1;
	}
	return -1;
}
int endDelimiter(const char *const code) {
	if (!((*code) && IS_DELIMITER(*code))) {
		return 0;
	}
	return -1;
}

int beginNumber(const char *const code) {
	if (IS_NUMBER(*code)) {
		return 1;
	}
	return -1;
}
int endNumber(const char *const code) {
	if (!((*code) && IS_NUMBER(*code))) {
		return 0;
	}
	return -1;
}

int beginIdentifier(const char *const code) {
	if ((isalpha(*code)) || (*code == '_')) {
		return 1;
	}
	return -1;
}
int endIdentifier(const char *const code) {
	if (!((isalnum(*code)) || (*code == '_'))) {
		return 0;
	}
	return -1;
}

void printLineString(const char *string, const int length)
{
	for (int i = 0; i < length; i++, string++) {
		if ((*string == '\n') || (i >= MAX_STRING)) {
			printf("...");
			break;
		}
		putchar(*string);
	}
}

void printTokenList(const struct TokenList *const tokens)
{
	printf("Line\tCol\tToken Type\tValue\n");
	for (unsigned int tokenIndex = 0; tokenIndex < tokens->lastIndex; tokenIndex++) {
		if (tokens->list[tokenIndex].type != SPACE) {
			printf("%u\t", tokens->list[tokenIndex].line);
			printf("%u\t", tokens->list[tokenIndex].col);
			printf("%s\t", tokList[tokens->list[tokenIndex].type].name);
			printLineString(tokens->list[tokenIndex].string, tokens->list[tokenIndex].length);
			putchar('\n');
		}
	}
}

void printColoredCode(const char *code, const struct TokenList *const tokens)
{
	unsigned int tokenIndex = 0;
	while (*code) {
		if (code >= tokens->list[tokenIndex].string) {
			printf("%s", tokList[tokens->list[tokenIndex].type].color);
			tokenIndex++;
		}
		putchar(*code);
		code++;
	}
}

struct TokenList lex(struct EnvironmentData *const env, const char *code)
{
	static const struct TokenCondition testTokCondition[TEST_COUNT] = {
		X_TEST_TOK(X_TEST_EXPAND_TABLE)
	};
	unsigned int line = 1, col = 1;
	struct TokenList tokens = {
		.list = NULL,
		.lastIndex = 0,
		.size = 0,
	};

	while (*code) {
		// Allocates more memory if necessary
		if (tokens.lastIndex >= tokens.size) {
			tokens.size = 2*MAX(tokens.size, 1);
			struct Token *newList = (struct Token *) realloc(tokens.list, tokens.size*sizeof(struct Token));
			if (!newList) {
				printError(env, "Lexical analysis error.\n");
				free(tokens.list);
				tokens.list = NULL;
				tokens.size = tokens.lastIndex = 0;
				break;
			}
			tokens.list = newList;
		}

		// Initializes next token
		tokens.list[tokens.lastIndex] = (struct Token) {
			.type = UNKNOWN,
			.string = code,
			.length = 0,
			.line = line,
			.col = col,
		};

		// Find type of next token
		for (unsigned int testIndex = 0; testIndex < TEST_COUNT; testIndex++) {

			// If found the token type
			int length = testTokCondition[testIndex].begin(code);
			if (length != -1) {
				tokens.list[tokens.lastIndex].type = testTokCondition[testIndex].type;
				code += length;
				col += length;

				// While isn't the end of current token
				while ((length = testTokCondition[testIndex].end(code)) == -1) {

					// Test for string error
					if (testTokCondition[testIndex].error) {
						char *msg = testTokCondition[testIndex].error(code);
						if (msg) {
							printError(env, msg);
							length = 0;
							break;
						}
					}

					// If the end of file is found in midle of a token
					if (!*code) {
						printError(env, "Didn't reach the end of current definition.\n");
						length = 0;
						break;
					}

					// Go to next character
					col++;
					if (*code == '\n') {
						line++;
						col = 1;
					}
					code++;
				}

				// Reach the end of current token
				code += length;
				col += length;
				tokens.list[tokens.lastIndex].length = code - tokens.list[tokens.lastIndex].string;
				break;
			}
		}

		// Particular case: an identifier can be a keyword
		if (tokens.list[tokens.lastIndex].type == IDENTIFIER) {
			if (isKeyword(tokens.list[tokens.lastIndex].string, tokens.list[tokens.lastIndex].length) != -1) {
				tokens.list[tokens.lastIndex].type = KEYWORD;
			}
		}

		// If no token is found
		if (tokens.list[tokens.lastIndex].type == UNKNOWN) {
			printError(env, "Ilegal definition.\n");
			break;
		}

		// Go to next token
		tokens.lastIndex++;
	}

	return tokens;
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
