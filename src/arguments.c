//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arguments.h"

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static const struct ArgCmd argList[ARG_COUNT] = {
	X_ARGUMENTS(X_ARG_EXPAND_INIT_TABLE)
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

void setFileName(struct ArgParameters *this, struct EnvironmentData *env)
{
	if (this->parameter) {
		if (strstr(this->parameter, ".js")) {
			// Save name of the file
			env->fileName = this->parameter;
		} else {
			printError(env, "The file especified %s must be at format \'.js\'.\n", this->parameter);
		}
	} else {
		printError(env, "You must specify a script after using the option %s.\n", this->argument);
	}
}

void printVersion(struct ArgParameters *this, struct EnvironmentData *env)
{
	printf("Version: %s\n", env->version);
}

void printUsage(struct ArgParameters *this, struct EnvironmentData *env)
{
	printf("Usage: %s [Options] [-f script.js | --file script.js | script.js]\n", env->software);
	printf("\n[Options]:\n");
	for (unsigned int arg = 0; arg < ARG_COUNT; arg++) {
		printf("   -%c, or %-10s\t%s\n", argList[arg].cmd[2], argList[arg].cmd, argList[arg].usage);
	}
}

void setLexOption(struct ArgParameters *this, struct EnvironmentData *env)
{
	if (!env->fileName) {
		printError(env, "You need to load a script file to use %s option.\n", this->argument);
	} else {
		env->flag.lex = true;
	}
}

int searchArgument(char *argument)
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

struct EnvironmentData parseArguments(int argc, char *argv[])
{
	int argIndex, argFound;
	char *argument, *parameter;
	struct EnvironmentData env = {
		.software = argv[0],
		.version = __VERSION,
		.fileName = NULL,
		.flag = {0},
	};
	struct ArgParameters argParameters[ARG_COUNT] = {NULL};

	if (argc >= 2) {
		for (argIndex = 1; argIndex < argc; argIndex++) {
			argument = argv[argIndex];

			if (argument[0] != '-') {
				parameter = argument;
				argument = "-f";
			} else {
				parameter = argv[argIndex+1];
			}

			argFound = searchArgument(argument);
			if (argFound >= 0) {
				if (argList[argFound].param) {
					if (argParameters[argFound].argument) {
						printError(&env, "You can't specify the %s option twice.\n", argument);
						break;
					} else {
						argParameters[argFound].parameter = parameter;
						argIndex++;
					}
				}
				argParameters[argFound].argument = argument;
			} else {
				printError(&env, "Unknown command: %s.\n", argument);
				break;
			}
		}
	} else {
		printUsage(NULL, &env);
		env.flag.error = true;
	}

	// Run functions especified in the command line
	if (!env.flag.error) {
		for (argIndex = 0; argIndex < ARG_COUNT; argIndex++) {
			if (argParameters[argIndex].argument) {
				argList[argIndex].function(&argParameters[argIndex], &env);
			}
		}
	}

	return env;
}
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
