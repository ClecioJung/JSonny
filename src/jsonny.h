#ifndef __JSONNY
#define __JSONNY

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdbool.h>

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

#define ERROR_MSG						"Error: "

// Software version
#define __VERSION						"1.0.0"

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

// Flags used to pass information to main function
struct FlagOptions {
	bool error;
	bool lex;
};

// Enviroment data passed to command functions
struct EnvironmentData {
	char *software;
	char *version;
	char *fileName;
	struct FlagOptions flag;
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

char *getContentFromFile(char *name);
void printError(struct EnvironmentData *env, const char *msg, ...);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __JSONNY
