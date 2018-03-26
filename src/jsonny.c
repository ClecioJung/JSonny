//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "jsonny.h"
#include "arguments.h"

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

char *getContentFromFile(char *name)
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

void printError(struct EnvironmentData *env, const char* msg, ...)
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

int main(int argc, char *argv[])
{
	struct EnvironmentData env = parseArguments(argc, argv);
	if (env.flag.error) {
		return EXIT_FAILURE;
	} else {

	}

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
