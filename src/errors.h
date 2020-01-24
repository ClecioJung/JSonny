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
void printCrash(const char *const msg, ...);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __ERRORS