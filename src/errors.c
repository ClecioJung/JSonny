//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "definitions.h"
#include "errors.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static bool crashFlag = false;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

// Function used to turn private to this file the error flag
bool isSomethingWrong(void)
{
	return crashFlag;
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

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
