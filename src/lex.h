#ifndef __LEX
#define __LEX

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <regex.h>
#include "commands.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

#define IS_NEW_LINE(X)					((X) == '\n')
#define IS_SPACE(X)						(((X) == ' ') || ((X) == '\t') || ((X) == '\n') || ((X) == '\r'))

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

struct Token {
  enum CommandType type;
  char *string;
  size_t length;
  unsigned int line;
  unsigned int col;
};

struct TokenList {
  struct Token *list;
  size_t size;
};

//------------------------------------------------------------------------------
// FUNCTIONS JSONNY.C
//------------------------------------------------------------------------------

void printTokenList(struct TokenList *tokList);
void *reallocFree(void *ptr, size_t size);
size_t testRegExp(char *regExp, char *string);
struct TokenList lex(char *code, struct Command commandTable[]);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __LEX
