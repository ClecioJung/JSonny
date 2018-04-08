#ifndef __DELIMITERS
#define __DELIMITERS

//------------------------------------------------------------------------------
// LIBRARIES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

/*  List of interpretable delimiters
- The order is relevant, example: ++ must come before +
- Example:
X(delimiter,		STATE_NAME)
*/
#define X_DELIMITERS(X) \
X("++",				INCREMENT 				) \
X("--",				DECREMENT 				) \
X("+",				PLUS 						) \
X("-",				MINUS 					) \
X("*",				MULTIPLICATION			) \
X("/",				DIVISION 				) \
X("%%",				MODULUS 					) \
X("!==",				NOT_EQUAL_STRICT		) \
X("!=",				NOT_EQUAL 				) \
X("===",				EQUAL_STRICT 			) \
X("==",				EQUAL 					) \
X("+=",				ADD_ASSIGN 				) \
X("-=",				SUB_ASSIGN 				) \
X("*=",				MUL_ASSIGN 				) \
X("/=",				DIV_ASSIGN 				) \
X("%%=",				MOD_ASSIGN 				) \
X(">=",				GREATHER_EQUAL 		) \
X("<=",				LESS_EQUAL 				) \
X("=",				ASSIGNMENT 				) \
X(">>>",				RIGHT_SHIFT 			) \
X("<<",				LEFT_SHIFT 				) \
X(">>",				SIGNED_RIGHT_SHIFT	) \
X(">",				GREATHER_THAN 			) \
X("<",				LESS_THAN 				) \
X("&&",				LOCICAL_AND 			) \
X("||",				LOCICAL_OR 				) \
X("!",				LOCICAL_NOT 			) \
X("&",				AND 						) \
X("|",				OR 						) \
X("~",				NOT 						) \
X("^",				XOR 						) \
X("?",				TERNARY 					) \
X(":",		 		COLON 					) \
X("{",		 		OPEN_BRACKETS 			) \
X("}",		 		CLOSE_BRACKETS 		) \
X("(",	 			OPEN_PARENTHESIS 		) \
X(")",		 		CLOSE_PARENTHESIS 	) \
X(";",		 		SEMICOLON 				) \
X(",",		 		COMMA 					) \
X(".",		 		DOT 						)

// Auxiliar definitions for keywords X MACRO
#define X_DELIM_EXPAND_TABLE(a,b)		a,
#define X_DELIM_EXPAND_TYPE(a,b)			b,
#define X_DELIM_EXPAND_COUNT(a,b)		+1
#define DELIM_COUNT							(0 X_DELIMITERS(X_DELIM_EXPAND_COUNT))

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum DelimiterType {
	X_DELIMITERS(X_DELIM_EXPAND_TYPE)
};

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

int isDelimiter(const char *const string, int *const length);

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __DELIMITERS
