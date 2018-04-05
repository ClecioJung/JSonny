//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "definitions.h"
#include "jsonny.h"
#include "arguments.h"
#include "lex.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

char *getContentFromFile(const char *const name)
{
	char *buffer = NULL;
	size_t fileSize, result;
	FILE *file = NULL;

	file = fopen(name, "r");
	if (file != NULL) {

		// Get file size
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		rewind(file);

		// Allocate memory to store the entire file
		buffer = (char *) malloc(fileSize*sizeof(char));
		if (buffer != NULL) {

			// Copy the contents of the file to the buffer
			result = fread(buffer, 1, fileSize, file);
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

void printError(struct EnvironmentData *const env, const char *const msg, ...)
{
	va_list args;
	char *str;
	size_t length;

	// Print Error in the begining of the message
	length = strlen(ERROR_MSG) + strlen(msg) + 1;
	str = (char *) malloc(length);
	strcpy(str, ERROR_MSG);
	strcat(str, msg);

	// Print message into stderr
	va_start (args, msg);
	vfprintf (stderr, str, args);
	va_end (args);

	free(str);

	// Set error flag
	env->flag.error = true;
}

//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------

int main(const int argc, const char *const argv[])
{
	char *prog = NULL;
	struct TokenList tokens = {
		.list = NULL,
	};

	struct EnvironmentData env = parseArguments(argc, argv);
	if ((!env.flag.error) && (env.flag.file)) {
		// Read data from file
		prog = getContentFromFile(env.fileName);
		if (!prog) {
			printError(&env, "The specified file couldn't be loaded.\n");
		} else {
			// Lexical analysis
			tokens = lex(&env, prog);

			// Run options specified by user
			if (env.flag.lex) {
				printTokenList(&tokens);
			} else if (env.flag.print) {
				printColoredCode(prog, &tokens);
			}
		}
	}

	// Free alocated memory and exit
	free(prog);
	free(tokens.list);
	if (env.flag.error) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
