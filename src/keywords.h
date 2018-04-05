#ifndef __KEYWORDS
#define __KEYWORDS

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

/*  List of interpretable keywords
- Example:
X(keywords,		STATE_NAME)
*/
#define X_KEYWORDS(X) \
X("break",		BREAK		) \
X("case",		CASE		) \
X("catch",		CATCH		) \
X("const",		CONST		) \
X("continue",	CONTINUE	) \
X("default",	DEFAULT		) \
X("delete",		DELETE 		) \
X("do",			DO 			) \
X("else",		ELSE 		) \
X("false",		FALSE 		) \
X("final",		FINAL 		) \
X("finally",	FINALLY 	) \
X("for",		FOR 		) \
X("function",	FUNCTION 	) \
X("if",			IF 			) \
X("in",			IN 			) \
X("instanceof",	INSTANCEOF 	) \
X("let",		LET 		) \
X("new",		NEW 		) \
X("return",		RETURN 		) \
X("switch",		SWITCH 		) \
X("this",		THIS 		) \
X("throw",		THROW 		) \
X("true",		TRUE 		) \
X("try",		TRY 		) \
X("typeof",		TYPEOF 		) \
X("var",		VAR 		) \
X("while",		WHILE 		)

// Auxiliar definitions for keywords X MACRO
#define X_KEY_EXPAND_TABLE(a,b)			a,
#define X_KEY_EXPAND_TYPE(a,b)			b,
#define X_KEY_EXPAND_COUNT(a,b)			+1
#define KEY_COUNT						(0 X_KEYWORDS(X_KEY_EXPAND_COUNT))

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum KeywordType {
	X_KEYWORDS(X_KEY_EXPAND_TYPE)
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

int isKeyword(const char *const string, const size_t length);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __KEYWORDS
