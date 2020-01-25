//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "definitions.h"
#include "errors.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static bool crashFlag = false;
static unsigned int errors = 0;
static unsigned int warnings = 0;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

// Function used to turn private to this file the error flag
bool isSomethingWrong(void)
{
	return crashFlag;
}

bool errorsInTheCode(void)
{
	return errors;
}

bool warningsInTheCode(void)
{
	return warnings;
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
	crashFlag = true;
}

void printError(struct Positioning pos, const char *const msg, ...)
{
	va_list args;
	const char *const startLine = pos.string - pos.col + 1;
	const unsigned int lineLength = strchr(startLine, '\n') - startLine;

	// Print header in stream
	fprintf(stderr, ERROR_HEADER "in line %u column %u: ", pos.line, pos.col);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);

	// Print code line to indicate where the error is
	fprintf(stderr, "%.*s\n" BOLD_FONT RED_FOREGROUND, lineLength, startLine);
	for (unsigned int columns = 0; columns < (pos.col-1); columns++) {
		if (isspace(startLine[columns])) {
			putc(startLine[columns], stderr);
		} else {
			putc(' ', stderr);
		}
	}
	fprintf(stderr, "^\n" RESET_FONT);

	// Increase the error counter
	errors++;
}

void printWarning(struct Positioning pos, const char *const msg, ...)
{
	va_list args;
	const char *const startLine = pos.string - pos.col + 1;
	const unsigned int lineLength = strchr(startLine, '\n') - startLine;

	// Print header in stream
	fprintf(stderr, WARNING_HEADER "in line %u column %u: ", pos.line, pos.col);

	// Print message into stream
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);

	// Print code line to indicate where the error is
	fprintf(stderr, "%.*s\n" BOLD_FONT MAGENTA_FOREGROUND, lineLength, startLine);
	for (unsigned int columns = 0; columns < (pos.col-1); columns++) {
		if (isspace(startLine[columns])) {
			putc(startLine[columns], stderr);
		} else {
			putc(' ', stderr);
		}
	}
	fprintf(stderr, "^\n" RESET_FONT);

	// Increase the warning counter
	warnings++;
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
