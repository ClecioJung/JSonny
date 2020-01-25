#ifndef __ERRORS
#define __ERRORS

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include "lex.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

bool isSomethingWrong(void);
bool errorsInTheCode(void);
bool warningsInTheCode(void);
void printCrash(const char *const msg, ...);
void printError(struct Positioning pos, const char *const msg, ...);
void printWarning(struct Positioning pos, const char *const msg, ...);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __ERRORS
