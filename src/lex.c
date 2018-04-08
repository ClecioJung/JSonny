//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"
#include "jsonny.h"
#include "print.h"
#include "keywords.h"
#include "delimiters.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

// Macro used in lex() to simpify the code
#define LAST_TOKEN(ENV)					((ENV)->tokens.list[(ENV)->tokens.lastIndex])

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

static int beginWhiteSpace(const char *const code) {
	if (IS_SPACE(*code)) {
		return 0;
	}
	return -1;
}
static int endWhiteSpace(const char *const code) {
	if (!((*code) && IS_SPACE(*code))) {
		return 0;
	}
	return -1;
}

static int beginMultiLineComment(const char *const code) {
	if (!strncmp(code, "/*", 2)) {
		return 2;
	}
	return -1;
}
static int endMultiLineComment(const char *const code) {
	if (!strncmp(code, "*/", 2)) {
		return 2;
	}
	return -1;
}

static int beginLineComment(const char *const code) {
	if (!strncmp(code, "//", 2)) {
		return 2;
	}
	return -1;
}
static int endLineComment(const char *const code) {
	if ((*code == '\n') && (*(code-1) != '\\')) {
		return 0;
	}
	return -1;
}

static int isString(const char *const code) {
	if (*code == '"') {
		return 1;
	}
	return -1;
}
static int isApostropheString(const char *const code) {
	if (*code == '\'') {
		return 1;
	}
	return -1;
}
static char *testString(const char *const code) {
	if ((*code == '\n') && (*(code-1) != '\\')) {
		return "String not properly defined.\n";
	}
	return NULL;
}

static int beginNumber(const char *const code) {
	if (IS_NUMBER(*code)) {
		return 1;
	}
	return -1;
}
static int endNumber(const char *const code) {
	if (!((*code) && IS_FLOAT_NUMBER(*code))) {
		return 0;
	}
	return -1;
}

static int beginIdentifier(const char *const code) {
	if ((isalpha(*code)) || (*code == '_')) {
		return 1;
	}
	return -1;
}
static int endIdentifier(const char *const code) {
	if (!((isalnum(*code)) || (*code == '_'))) {
		return 0;
	}
	return -1;
}

static inline size_t max(const size_t x, const size_t y)
{
	return (((x) > (y)) ? (x) : (y));
}

static inline bool realocateTokenList(struct TokenList *const tokens)
{
	if (tokens->lastIndex >= tokens->size) {
		tokens->size = 2*max(tokens->size, 1);
		struct Token *newList = (struct Token *) realloc(tokens->list, tokens->size*sizeof(struct Token));
		if (!newList) {
			free(tokens->list);
			tokens->list = NULL;
			tokens->size = tokens->lastIndex = 0;
			return false;
		}
		tokens->list = newList;
	}
	return true;
}

/* Treat particular cases and return true if the token was unknown and
discovered here, so that the calling function can update next character
position, because the token length was also discovered */
static inline bool treatParticularTokens(struct Token *const token)
{
	// An identifier can be a keyword
	if (token->type == IDENTIFIER) {
		token->subToken = isKeyword(token->string, token->length);
		if (token->subToken != -1) {
			token->type = KEYWORD;
		}
	}
	// If the token is not known yet, is length is also unknown, so return true
	else if (token->type == UNKNOWN) {
		// If no token mathes, can be a delimiter
		token->subToken = isDelimiter(token->string, &token->length);
		if (token->subToken != -1) {
			token->type = DELIMITER;
			return true;
		}
	}

	return false;
}

void lex(struct EnvironmentData *const env)
{
	unsigned int line = 1, col = 1;
	static const struct TokenCondition testTokCondition[TEST_COUNT] = {
		X_TEST_TOK(X_TEST_EXPAND_TABLE)
	};

	for (const char *code = env->prog; *code; env->tokens.lastIndex++) {

		// Allocates more memory if necessary
		if (!realocateTokenList(&env->tokens)) {
			printCrash("Lexical analysis error.\n");
			break;
		}

		// Initializes next token
		LAST_TOKEN(env) = (struct Token) {
			.type = UNKNOWN,
			.string = code,
			.length = 0,
			.line = line,
			.col = col,
			.subToken = 0,
		};

		// Find type of next token
		for (unsigned int testIndex = 0; testIndex < TEST_COUNT; testIndex++) {

			// If found the token type
			int length = testTokCondition[testIndex].begin(code);
			if (length != -1) {
				LAST_TOKEN(env).type = testTokCondition[testIndex].type;
				col += length;
				code += length;

				// While isn't the end of current token
				while ((length = testTokCondition[testIndex].end(code)) == -1) {

					// Test for string error
					if (testTokCondition[testIndex].error) {
						char *msg = testTokCondition[testIndex].error(code);
						if (msg) {
							printError(LAST_TOKEN(env).line, LAST_TOKEN(env).col, msg);
							length = 0;
							break;
						}
					}

					// If the end of file is found in midlle of a token
					if (!*code) {
						printError(LAST_TOKEN(env).line, LAST_TOKEN(env).col, "Didn't reach the end of current definition.\n");
						length = 0;
						break;
					}

					// Go to next character
					col++;
					if (IS_NEW_LINE(*code)) {
						line++;
						col = 1;
					}
					code++;
				}

				// Reach the end of current token
				col += length;
				code += length;
				LAST_TOKEN(env).length = code - LAST_TOKEN(env).string;
				break;
			}
		}

		// Treat particular cases
		if (treatParticularTokens(&LAST_TOKEN(env))) {
			col += LAST_TOKEN(env).length;
			code += LAST_TOKEN(env).length;
		}

		// If the token is still unknown, break the loop
		if (LAST_TOKEN(env).type == UNKNOWN) {
			printError(LAST_TOKEN(env).line, LAST_TOKEN(env).col, "Ilegal definition.\n");
			break;
		}
	}
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
