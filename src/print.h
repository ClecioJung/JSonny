#ifndef __PRINT
#define __PRINT

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include "definitions.h"
#include "lex.h"

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

// Header to anounce to the user the meaning of current message
#define CRASH_HEADER							BOLD_FONT RED_FOREGROUND "[Crash] " RESET_FONT
#define ERROR_HEADER							BOLD_FONT RED_FOREGROUND "[Error] " RESET_FONT
#define WARNING_HEADER						BOLD_FONT MAGENTA_FOREGROUND "[Warning] " RESET_FONT
#define USAGE_HEADER							BOLD_FONT BLUE_FOREGROUND "[Usage] " RESET_FONT
#define VERSION_HEADER						BOLD_FONT BLUE_FOREGROUND "[Version] " RESET_FONT

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

bool isSomethingWrong(void);
void printCrash(const char *const msg, ...);
void printError(const unsigned int line, const unsigned int col, const char *const msg, ...);
void printWarning(const unsigned int line, const unsigned int col, const char *const msg, ...);
void printUsage(const char *const software);
void printVersion(void);
void printTokenList(struct TokenList *const tokens);
void printColoredCode(const char *code, struct TokenList *const tokens);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __PRINT
