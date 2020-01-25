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
#include "errors.h"
#include "lex.h"
#include "parser.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

typedef void (*ArgFunction)(void);

// Table used to concentrate all the information related to the comand line arguments
struct ArgCmd {
	char cmd;
	const char *command;
	ArgFunction function;
	const char *usage;
};

enum ActionToBeTaken {
	acNone = 0,
	acPrintTokenList,
	acPrintColoredCode,
	acParser,
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

void printUsage(void);
void printVersion(void);
void printAbout(void);
void debugModeOn(void);
void setActionToken(void);
void setActionCode(void);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static const struct ArgCmd argList[] = {
	{'h',	"--help",			printUsage,			"Display this help message."},
	{'v',	"--version",	printVersion,		"Display the software version."},
	{'a',	"--about",		printAbout,			"Display information about the software."},
	{'d',	"--debug",		debugModeOn,		"Turns the debug mode on."},
	{'l',	"--lex",			setActionToken,	"Display the lexical analysis of the input."},
	{'c',	"--code",			setActionCode,	"Display the colored code."},
	{0}
};
static const char *software = NULL;
static bool debug = false;
static enum ActionToBeTaken action = acParser;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

void printUsage(void) {
	printf(USAGE_HEADER "%s [script.js] [Options]\n", software);
	printf("\n[Options]:\n");
	for (unsigned int argIdx = 0; argList[argIdx].cmd; argIdx++) {
		printf("   -%c, or %-10s\t%s\n", argList[argIdx].cmd, argList[argIdx].command, argList[argIdx].usage);
	}
	action = acNone;
}

void printVersion(void) {
	printf(VERSION_HEADER VERSION "\n");
	printf("Compilation date: %s\n", __DATE__);
	action = acNone;
}

void printAbout(void) {
	printf(ABOUT_HEADER "A simple JavaScript interpreter written in C.\n");
	action = acNone;
}

void debugModeOn(void) {
	printf(SETTINGS_HEADER "Debug mode on.\n");
	debug = true;
}

void setActionToken(void) {
	action = acPrintTokenList;
}

void setActionCode(void) {
	action = acPrintColoredCode;
}

static bool parseArgument(const char *const arg) {
	if (arg[0] == '-') {
		const size_t length = strlen(arg);
		// make sequential search looking for the argument
		for (unsigned int argIdx = 0; argList[argIdx].cmd; argIdx++) {
			if ((!strcmp(arg, argList[argIdx].command)) ||
			((length == 2) && (arg[1] == argList[argIdx].cmd))) {
				// found the argument in the list
				argList[argIdx].function();
				return true;
			}
		}
		// no valid argument found
		printCrash("Unknown argument: %s\n", arg);
	}
	return false;
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

//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------

int main(const int argc, const char *const argv[]) {
	const char *fileName = NULL;
	const char *fileContends = NULL;
	struct TokenList *tokens = NULL;
	software = argv[0];

	// Parse command line arguments
	for (int argIdx = 1; argIdx < argc; argIdx++) {
		if (!parseArgument(argv[argIdx])) {
			if (isSomethingWrong() || fileName) {
				printUsage();
				return EXIT_FAILURE;
			} else {
				// if isn't a argument, only can be a file name
				fileName = argv[argIdx];
			}
		}
	}

	// Load script from file
	if (action != acNone) {
		if (fileName) {
			fileContends = getContentFromFile(fileName);
			if (!fileContends) {
				printCrash("The specified file couldn't be loaded.\n");
				return EXIT_FAILURE;
			}
		} else {
			printCrash("No file specified.\n");
			printUsage();
			return EXIT_FAILURE;
		}
	}

	if (action >= acPrintTokenList) {
		tokens = lex(fileContends);
	}

	// Executes the required action
	switch (action) {
		case acNone:
		return EXIT_SUCCESS;
		case acPrintTokenList:
		printTokenList(tokens);
		break;
		case acPrintColoredCode:
		printColoredCode(fileContends, tokens);
		break;
		case acParser:
		default:
		parser(tokens);
	}

	// Free alocated memory and exit
	free((void*)fileContends);
	free((void*)tokens->list);
	free((void*)tokens);
	freeParser();
	return (isSomethingWrong() ? EXIT_FAILURE : EXIT_SUCCESS);
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
