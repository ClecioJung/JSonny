//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include "lex.h"

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

void printTokenList(struct TokenList *tokens)
{
	unsigned int tokenIndex;
	const char *commandName[] = {
		X_COMMANDS(X_CMD_EXPAND_NAMES)
	};

	printf("Line\tCol\tCommand\n");
	for (tokenIndex = 0; tokenIndex < tokens->size; tokenIndex++) {
		printf("%u\t%u\t%s\n", tokens->list[tokenIndex].line, tokens->list[tokenIndex].col,
		commandName[tokens->list[tokenIndex].type]);
	}
}

/* This function is a wrapper for standart realloc() with one difference -
it frees old memory pointer in case of realloc failure.
*/
void *reallocFree(void *ptr, size_t size) {
	void *p = realloc(ptr, size);
	if (!p)  {
		free (ptr);
		// fprintf(stderr, "realloc(): errno=%d\n", errno);
	}
	return p;
}

size_t testRegExp(char *regExp, char *string)
{
	regmatch_t match;
	regex_t reg;

	if (!regcomp(&reg, regExp, REG_EXTENDED)) {
		if (!regexec(&reg, string, 1, &match, 0)) {
			// Return the length of the expression found
			return (match.rm_eo - match.rm_so);
		}
	}

	return 0;
}

struct TokenList lex(char *code, struct Command commandTable[])
{
	size_t regExpLength;
	unsigned int cmd, line = 0, col = 0;
	struct TokenList tokens = {
		.list = NULL,
		.size = 0,
	};

	while(*code) {
		while (IS_SPACE(*code)) {
			col++;
			if (IS_NEW_LINE(*code)) {
				line++;
				col = 0;
			}
			code++;
		}

		if (*code) {
			for (cmd = 0; cmd < CMD_COUNT; cmd++) {
				regExpLength = testRegExp(commandTable[cmd].regExp, code);
				if (regExpLength) {

					tokens.list = (struct Token *) reallocFree(tokens.list, (++tokens.size)*sizeof(struct Token));
					if (!tokens.list) {
						tokens.size = 0;
						return tokens;
					}

					// Command found
					tokens.list[tokens.size-1] = (struct Token) {
						.type = cmd,
						.string = code,
						.length = regExpLength,
						.line = line,
						.col = col,
					};
					code += regExpLength;
					col += regExpLength;
					break;
				}
			}

			// Error: no command fits the code expression
			if (cmd >= CMD_COUNT) {
				printf("error no command fits the code expression\n");
				tokens.size = 0;
				free(tokens.list);
				return tokens;
			}
		}
	}

	return tokens;
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
