//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jsonny.h"
#include "arguments.h"
#include "print.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

static inline char *getContentFromFile(const char *const name)
{
	char *buffer = NULL;
	size_t fileSize, result;
	FILE *file;

	file = fopen(name, "r");
	if (file) {

		// Get file size
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		rewind(file);

		// Allocate memory to store the entire file
		buffer = (char *) malloc((fileSize+1)*sizeof(char));
		if (buffer) {

			// Copy the contents of the file to the buffer
			result = fread(buffer, sizeof(char), fileSize, file);
			buffer[fileSize] = '\0';
			if (result != fileSize) {

				// Reading file error, free dinamically allocated memory
				free(buffer);
				buffer = NULL;
			}
		}
		fclose(file);
	}
	return buffer;
}

static inline void loadFile(const char *const name, struct EnvironmentData *const env)
{
	if (strstr(name, ".js")) {
		env->fileName = name;
		env->prog = getContentFromFile(env->fileName);
		if (!env->prog) {
			printCrash("The specified file couldn't be loaded.\n");
		}
	} else {
		printCrash("The file especified %s must be at format \'.js\'.\n", name);
	}
}

//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------

int main(const int argc, const char *const argv[])
{
	struct EnvironmentData env = {
		.software = argv[0],
		.fileName = NULL,
		.prog = NULL,
		.tokens = {
			.list = NULL,
			.lastIndex = 0,
			.size = 0,
		},
	};

	if (argc < 2) {
		printCrash("You must specify the action to be taken by command line.\n");
	} else {
		int argIndex = 1;
		if (argv[argIndex][0] != '-') {
			loadFile(argv[argIndex], &env);
			argIndex++;
		}
		if (argIndex < (argc-1)) {
			printCrash("To many arguments in command line.\n");
		} else if (!isSomethingWrong()) {
			if (argv[argIndex]) {
				int argFound = searchArgument(argv[argIndex]);
				if (argFound >= 0) {
					argList[argFound].function(argv[argIndex], &env);
				} else {
					printCrash("Unknown command: %s.\n", argv[argIndex]);
				}
			} else {
				// Default action
				argPrintCode("this", &env);
			}
		}
	}

	// Free alocated memory and exit
	free(env.prog);
	free(env.tokens.list);
	if (isSomethingWrong()) {
		printUsage(env.software);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
