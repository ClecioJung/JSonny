//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include "definitions.h"
#include "arguments.h"
#include "errors.h"
#include "lex.h"
#include "parser.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum ActionToBeTaken {
	acNone = 0,
	acPrintTokenList,
	acPrintColoredCode,
	acParser,
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static bool debug = false;
static enum ActionToBeTaken action = acParser;
static const char *fileName = NULL;
static const char *fileContends = NULL;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

static int getFileName(const char *const arg) {
	if (fileName) {
		return EXIT_FAILURE;
	} else {
		fileName = arg;
	}
	return EXIT_SUCCESS;
}

static int printUsage(const char *const software) {
	printf(USAGE_HEADER "%s [script.js] [Options]\n", software);
	action = acNone;
	return EXIT_SUCCESS;
}

static int printVersion(const char *const arg) {
	(void) arg;

	printf(VERSION_HEADER VERSION "\n");
	printf("Compilation date: %s\n", __DATE__);
	action = acNone;
	return EXIT_SUCCESS;
}

static int printAbout(const char *const arg) {
	(void) arg;
	printf(ABOUT_HEADER "A simple JavaScript interpreter written in C.\n");
	action = acNone;
	return EXIT_SUCCESS;
}

static int debugModeOn(const char *const arg) {
	(void) arg;
	printf(SETTINGS_HEADER "Debug mode on.\n");
	debug = true;
	return EXIT_SUCCESS;
}

static int setActionToken(const char *const arg) {
	(void) arg;
	action = acPrintTokenList;
	return EXIT_SUCCESS;
}

static int setActionCode(const char *const arg) {
	(void) arg;
	action = acPrintColoredCode;
	return EXIT_SUCCESS;
}

static char *getContentFromFile(const char *const name) {
	char *buffer = NULL;

	// Get file size
	struct stat st;
	if(!stat(name, &st)) {
		const size_t fileSize = st.st_size;

		// Open file
		FILE *file = fopen(name, "rb");
		if (file) {
			// Allocate memory to store the entire file
			buffer = (char *) malloc((fileSize+1)*sizeof(char));
			if (buffer) {
				// Copy the contents of the file to the buffer
				const size_t result = fread(buffer, sizeof(char), fileSize, file);
				buffer[fileSize] = '\0';
				if (result != fileSize) {
					// Reading file error, free dinamically allocated memory
					free(buffer);
					buffer = NULL;
				}
			}
			fclose(file);
		}
	}
	return buffer;
}

static void freeMemory(void) {
	free((void*)fileContends);
	fileContends = NULL;
}

//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------

int main(const int argc, const char *const argv[]) {
	struct TokenList *tokens = NULL;
	atexit(freeMemory);

	// Treat main() arguments
	initArguments(printUsage, getFileName);
	addArgument("--version", NULL, printVersion, "Display the software version.");
	addArgument("--about", "-a", printAbout, "Display information about the software.");
	addArgument("--debug", "-d", debugModeOn, "Turns the debug mode on.");
	addArgument("--lex", "-l", setActionToken, "Display the lexical analysis of the input.");
	addArgument("--code", "-c", setActionCode, "Display the colored code.");
	parseArguments(argc, argv);

	// Load script from file
	if (action != acNone) {
		if (fileName) {
			fileContends = getContentFromFile(fileName);
			if (!fileContends) {
				printCrashAndExit("The specified file couldn't be loaded.\n");
			}
		} else {
			argumentsUsage("No file specified");
			return EXIT_FAILURE;
		}
	}

	// Executes the required action
	switch (action) {
		case acNone:
		return EXIT_SUCCESS;
		case acPrintTokenList:
		tokens = lex(fileContends);
		printTokenList(tokens);
		break;
		case acPrintColoredCode:
		tokens = lex(fileContends);
		printColoredCode(fileContends, tokens);
		break;
		case acParser:
		default:
		tokens = lex(fileContends);
		parser(tokens);
	}

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
