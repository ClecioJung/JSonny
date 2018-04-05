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
- Insert commands in the order to be executed
- Example:
X(name,		parameter,	function,		"usage")
*/
#define X_ARGUMENTS(X) \
X(version,	false,		printVersion,	"Print software version.") \
X(help,		false,		printUsage,		"Print this help message.") \
X(file,		true,		setFileName,	"Especifies the file to be evaluated.") \
X(lex,		false,		setLexOption,	"Print in the screen the lexical analysis of the input.") \
X(print,	false,		setPrintOption,	"Print in the screen the colored code.")

// Auxiliar definitions for arguments X MACRO
#define X_ARG_EXPAND_TABLE(a,b,c,d)		{.cmd = "--" # a, .param = b, .function = c, .usage = d},
#define X_ARG_EXPAND_COUNT(a,b,c,d)		+1
#define ARG_COUNT						(0 X_ARGUMENTS(X_ARG_EXPAND_COUNT))

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

// Struct used to save the arguments received from command line
struct ArgParameters {
	char *argument;
	char *parameter;
};

typedef void (*ArgFunction)(const struct ArgParameters *const, struct EnvironmentData *);

// Table used to concentrate all the information related to the commands
struct ArgCmd {
	char *cmd;
	ArgFunction function;
	char *usage;
	bool param;
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

struct EnvironmentData parseArguments(const int argc, const char *const argv[]);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __ARGUMENTS
