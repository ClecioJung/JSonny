//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <string.h>
#include "keywords.h"

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static const char *keyList[KEY_COUNT] = {
	X_KEYWORDS(X_KEY_EXPAND_TABLE)
};

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

int isKeyword(const char *const string, const size_t length)
{
	// Sequential search in the list of keywords
	for (int keyIndex = 0; keyIndex < KEY_COUNT; keyIndex++) {
		if (length == strlen(keyList[keyIndex])) {
			if (!strncmp(string, keyList[keyIndex], length)) {
				return keyIndex;
			}
		}
	}
	// Isn't a keyword
	return -1;
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
