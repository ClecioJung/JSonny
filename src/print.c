//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "print.h"
#include "arguments.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static bool error = false;

/* This table stores information about each token type,
and is defined here because is used only by
printTokenList() and printColoredCode() */
static const struct LexToken tokList[TOK_COUNT] = {
	X_TOKENS(X_TOK_EXPAND_TABLE)
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

// Function used to turn private to this file the error flag
bool isSomethingWrong(void)
{
	return error;
}

void printCrash(const char *const msg, ...)
{
	va_list args;

	// Print header in stream
	fputs(CRASH_HEADER, stderr);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);

	// Set error flag
	error = true;
}

void printError(const unsigned int line, const unsigned int col, const char *const msg, ...)
{
	va_list args;

	// Print header in stream
	fprintf(stderr, ERROR_HEADER "in line %u column %u: ", line, col);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
}

void printWarning(const unsigned int line, const unsigned int col, const char *const msg, ...)
{
	va_list args;

	// Print header in stream
	fprintf(stderr, WARNING_HEADER "in line %u column %u: ", line, col);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
}

void printUsage(const char *const software)
{
	printf("%s%s [script.js] [Options]\n", USAGE_HEADER, software);
	printf("\n[Options]:\n");
	for (unsigned int arg = 0; arg < ARG_COUNT; arg++) {
		printf("   -%c, or %-10s\t%s\n", argList[arg].cmd[2], argList[arg].cmd, argList[arg].usage);
	}
}

void printVersion(void)
{
	printf("%s%s\n", VERSION_HEADER, __VERSION);
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

void printTokenList(struct TokenList *const tokens)
{
	printf("Line\tColumn\tToken Type\tValue\n");
	for (unsigned int tokenIndex = 0; tokenIndex < tokens->lastIndex; tokenIndex++) {
		if (tokens->list[tokenIndex].type != SPACE) {
			fputs(tokList[tokens->list[tokenIndex].type].font, stdout);
			printf("%u\t", tokens->list[tokenIndex].line);
			printf("%u\t", tokens->list[tokenIndex].col);
			printf("%-15s\t", tokList[tokens->list[tokenIndex].type].name);
			printLineString(tokens->list[tokenIndex].string, tokens->list[tokenIndex].length);
			puts(RESET_FONT);
		}
	}
}

void printColoredCode(const char *code, struct TokenList *const tokens)
{
	for (unsigned int tokenIndex = 0; *code != '\0'; code++) {
		if (code >= tokens->list[tokenIndex].string) {
			printf(RESET_FONT "%s", tokList[tokens->list[tokenIndex].type].font);
			tokenIndex++;
		}
		putchar(*code);
	}
	printf(RESET_FONT);
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
