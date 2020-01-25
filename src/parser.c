//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include "lex.h"
#include "parser.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static int *heap = NULL;
static int *stack = NULL;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

void parser(const struct TokenList *const tokens) {
  for (unsigned int tkIndex = 0; tkIndex < tokens->size; tkIndex++) {
    // Ignores spaces, comments and unknown tokens
    if (tokens->list[tkIndex].type > TOK_COMMENT) {
      if (tokList[tokens->list[tkIndex].type].parse) {
        tokList[tokens->list[tkIndex].type].parse(tokens, tkIndex);
      }
    }
  }
}

void freeParser(void) {
  free((void*)heap);
  free((void*)stack);
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
