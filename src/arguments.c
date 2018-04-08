//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "arguments.h"
#include "lex.h"
#include "print.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

const struct ArgCmd argList[ARG_COUNT] = {
	X_ARGUMENTS(X_ARG_EXPAND_TABLE)
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

int searchArgument(const char *const argument)
{
	for (unsigned int argIndex = 0; argIndex < ARG_COUNT; argIndex++) {
		size_t length = strlen(argument);
		if (!strncmp(argument, argList[argIndex].cmd + (length == 2 ? 1 : 0), length)) {
			return argIndex;
		}
	}
	// If parameter not found, return -1
	return -1;
}

void argVersion(const char *const argument, struct EnvironmentData *const env)
{
	if (env->prog) {
		printCrash("You does't need to load a script file to use %s option.\n", argument);
	}
	printVersion();
}

void argHelp(const char *const argument, struct EnvironmentData *const env)
{
	if (env->prog) {
		printCrash("You does't need to load a script file to use %s option.\n", argument);
	} else {
		printUsage(env->software);
	}
}

void argPrintToken(const char *const argument, struct EnvironmentData *const env)
{
	if (!env->prog) {
		printCrash("You need to load a script file to use %s option.\n", argument);
	} else {
		lex(env);
		if (!isSomethingWrong()) {
			printTokenList(&env->tokens);
		}
	}
}

void argPrintCode(const char *const argument, struct EnvironmentData *const env)
{
	if (!env->prog) {
		printCrash("You need to load a script file to use %s option.\n", argument);
	} else {
		lex(env);
		if (!isSomethingWrong()) {
			printColoredCode(env->prog, &env->tokens);
		}
	}
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
