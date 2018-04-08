//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

#include <string.h>
#include "keywords.h"

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

int isKeyword(const char *const string, const size_t length)
{
	static const char *keyList[KEY_COUNT] = {
		X_KEYWORDS(X_KEY_EXPAND_TABLE)
	};

	// Sequential search in the list
	for (int index = 0; index < KEY_COUNT; index++) {
		if (length == strlen(keyList[index])) {
			if (!strncmp(string, keyList[index], length)) {
				return index;
			}
		}
	}
	// Wasn't found
	return -1;
}

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
