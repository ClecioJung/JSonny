#ifndef __ARGUMENTS
#define __ARGUMENTS

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdbool.h>
#include "jsonny.h"

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

/*  List of arguments interpreted by the main function
OBS: insert commands in the order to be executed
Exemple:
X(name,		parameter,	function,		"usage")
*/
#define X_ARGUMENTS(X) \
X(version,	false,		printVersion,	"Print software version.") \
X(help,		false,		printUsage,		"Print this help message.") \
X(file,		true,		setFileName,	"Especifies the file to be evaluated.") \
X(lex,		false,		setLexOption,	"Print in the screen the lexical analysis of the input.")

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

// Auxiliar definitions for arguments X MACRO
#define X_ARG_EXPAND_INIT_TABLE(a,b,c,d)	{.cmd = "--" # a, .param = b, .function = c, .usage = d},
#define X_ARG_EXPAND_COUNT(a,b,c,d)			+1
#define ARG_COUNT							(0 X_ARGUMENTS(X_ARG_EXPAND_COUNT))

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

// Struct used to save the arguments received from command line
struct ArgParameters {
	char *argument;
	char *parameter;
};

typedef void (*argFunction)(struct ArgParameters *, struct EnvironmentData *);

// Table used to concentrate all the information related to the commands
struct ArgCmd {
	char *cmd;
	argFunction function;
	char *usage;
	bool param;
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

void setFileName(struct ArgParameters *this, struct EnvironmentData *env);
void printVersion(struct ArgParameters *this, struct EnvironmentData *env);
void printUsage(struct ArgParameters *this, struct EnvironmentData *env);
void setLexOption(struct ArgParameters *this, struct EnvironmentData *env);

int searchArgument(char *argument);

struct EnvironmentData parseArguments(int argc, char *argv[]);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __ARGUMENTS
