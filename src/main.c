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

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

typedef void (*ArgFunction)(void);

// Table used to concentrate all the information related to the comand line arguments
struct ArgCmd {
	ArgFunction function;
	char *cmd;
	char *usage;
};

enum ActionToBeTaken {
	acNone = 0,
	acPrintColoredCode,
	acPrintTokenList,
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

const char *software = NULL;
const char *fileName = NULL;
const char *prog = NULL;
static bool debug = false;
enum ActionToBeTaken action = acPrintColoredCode;

static const struct ArgCmd argList[] = {
	{printUsage,			"--help",			"Display this help message."},
	{printVersion,		"--version",	"Display the software version."},
	{printAbout,			"--about",		"Display information about the software."},
	{debugModeOn,			"--debug",		"Turns the debug mode on."},
	{setActionToken,	"--lex",			"Display the lexical analysis of the input."},
	{setActionCode,		"--code",			"Display the colored code."},
};
static const unsigned int argCount = sizeof(argList)/sizeof(argList[0]);

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

void printUsage(void)
{
	printf(USAGE_HEADER "%s [script.js] [Options]\n", software);
	printf("\n[Options]:\n");
	for (unsigned int argIdx = 0; argIdx < argCount; argIdx++) {
		printf("   -%c, or %-10s\t%s\n", argList[argIdx].cmd[2], argList[argIdx].cmd, argList[argIdx].usage);
	}
	action = acNone;
}

void printVersion(void)
{
	printf(VERSION_HEADER __VERSION "\n");
	action = acNone;
}

void printAbout(void)
{
	printf(ABOUT_HEADER "A simple JavaScript interpreter written in C.\n");
	action = acNone;
}

void debugModeOn(void)
{
	printf(SETTINGS_HEADER "Debug mode on.\n");
	debug = true;
}

void setActionToken(void)
{
	action = acPrintTokenList;
}

void setActionCode(void)
{
	action = acPrintColoredCode;
}

static void parseArgument(const char *const arg)
{
	if (arg[0] == '-') {
		const size_t length = strlen(arg);
		// make sequential search looking for the argument
		for (unsigned int argIdx = 0; argIdx < argCount; argIdx++) {
			if ((!strcmp(arg, argList[argIdx].cmd)) ||
			((length == 2) && (arg[1] == argList[argIdx].cmd[2]))) {
				// found the argument in the list
				argList[argIdx].function();
				return;
			}
		}
		// if isn't a argument, only can be a file name
	} else if (!fileName) {
		fileName = arg;
		return;
	}
	// no valid argument found
	printCrash("Unknown argument: %s\n", arg);
}

static char *getContentFromFile(const char *const name)
{
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

int main(const int argc, const char *const argv[])
{
	software = argv[0];

	// Parse command line arguments
	for (int argIdx = 1; argIdx < argc; argIdx++) {
		parseArgument(argv[argIdx]);
		// If an error ocurred
		if (isSomethingWrong()) {
			printUsage();
			return EXIT_FAILURE;
		}
	}

	// Load script from file
	if (action != acNone) {
		if (fileName) {
			prog = getContentFromFile(fileName);
			if (!prog) {
				printCrash("The specified file couldn't be loaded.\n");
				return EXIT_FAILURE;
			}
		} else {
			printCrash("No file specified.\n");
			printUsage();
			return EXIT_FAILURE;
		}
	}

	// Executes the required action
	switch (action) {
		case acNone:
		return EXIT_SUCCESS;
		case acPrintTokenList:
		lex(prog);
		printTokenList();
		break;
		case acPrintColoredCode:
		default:
		lex(prog);
		printColoredCode(prog);
	}

	// Free alocated memory and exit
	free((void*)prog);
	freeLex();
	return (isSomethingWrong() ? EXIT_FAILURE : EXIT_SUCCESS);
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
