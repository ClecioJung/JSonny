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
X(name,		function,		"usage")
*/
#define X_ARGUMENTS(X) \
X(version,	argVersion,		"Print software version.") \
X(help,		argHelp,			"Print this help message.") \
X(lex,		argPrintToken,	"Print in the screen the lexical analysis of the input.") \
X(print,		argPrintCode,	"Print in the screen the colored code.")

// Auxiliar definitions for arguments X MACRO
#define X_ARG_EXPAND_TABLE(a,b,c)		{.cmd = "--" # a, .function = b, .usage = c},
#define X_ARG_EXPAND_COUNT(a,b,c)		+1
#define ARG_COUNT								(0 X_ARGUMENTS(X_ARG_EXPAND_COUNT))

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

typedef void (*ArgFunction)(const char *const, struct EnvironmentData *const);

// Table used to concentrate all the information related to the commands
struct ArgCmd {
	char *cmd;
	ArgFunction function;
	char *usage;
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

int searchArgument(const char *const argument);

void argVersion(const char *const argument, struct EnvironmentData *const env);
void argHelp(const char *const argument, struct EnvironmentData *const env);
void argPrintToken(const char *const argument, struct EnvironmentData *const env);
void argPrintCode(const char *const argument, struct EnvironmentData *const env);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

extern const struct ArgCmd argList[ARG_COUNT];

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __ARGUMENTS
