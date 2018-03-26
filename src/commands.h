#ifndef __COMMANDS
#define __COMMANDS

//------------------------------------------------------------------------------
// X MACRO
//------------------------------------------------------------------------------

/*  List of interpretable commands
You must enter commands in the order in which they are to be detected
Repeated tokens can be inserted for different commands
Exemple:
X(command,		STATE_NAME)
*/
#define X_COMMANDS(X) \
X("break",		_BREAK, 				) \
X("case",		_CASE, 					) \
X("catch",		_CATCH, 				) \
X("const",		_CONST, 				) \
X("continue",	_CONTINUE, 				) \
X("default",	_DEFAULT, 				) \
X("delete",		_DELETE, 				) \
X("do",			_DO, 					) \
X("else",		_ELSE, 					) \
X("false",		_FALSE, 				) \
X("final",		_FINAL, 				) \
X("finally",	_FINALLY, 				) \
X("for",		_FOR, 					) \
X("function",	_FUNCTION, 				) \
X("if",			_IF, 					) \
X("in",			_IN, 					) \
X("instanceof",	_INSTANCEOF, 			) \
X("let",		_LET, 					) \
X("new",		_NEW, 					) \
X("return",		_RETURN, 				) \
X("switch",		_SWITCH, 				) \
X("this",		_THIS, 					) \
X("throw",		_THROW, 				) \
X("true",		_TRUE, 					) \
X("try",		_TRY, 					) \
X("typeof",		_TYPEOF, 				) \
X("var",		_VAR, 					) \
X("while",		_WHILE, 				) \
X("++",			_INCREMENT, 			) \
X("--",			_DECREMENT, 			) \
X("+",			_ADDITION, 				) \
X("-",			_SUBTRACTION, 			) \
X("*",			_MULTIPLICATION,		) \
X("/",			_DIVISION, 				) \
X("%%",			_MODULUS, 				) \
X("!==",		_NOT_EQUAL_STRICT, 		) \
X("!=",			_NOT_EQUAL, 			) \
X("===",		_EQUAL_STRICT, 			) \
X("==",			_EQUAL, 				) \
X("+=",			_ADD_ASSIGN, 			) \
X("-=",			_SUB_ASSIGN, 			) \
X("*=",			_MUL_ASSIGN, 			) \
X("/=",			_DIV_ASSIGN, 			) \
X("%%=",		_MOD_ASSIGN, 			) \
X(">=",			_GREATHER_EQUAL, 		) \
X("<=",			_LESS_EQUAL, 			) \
X("=",			_ASSIGNMENT, 			) \
X("<<",			_LEFT_SHIFT, 			) \
X(">>",			_SIGNED_RIGHT_SHIFT,	) \
X(">>>",		_RIGHT_SHIFT, 			) \
X(">",			_GREATHER_THAN, 		) \
X("<",			_lESS_THAN, 			) \
X("&&",			_LOCICAL_AND, 			) \
X("||",			_LOCICAL_OR, 			) \
X("!",			_LOCICAL_NOT, 			) \
X("&",			_AND, 					) \
X("|",			_OR, 					) \
X("~",			_NOT, 					) \
X("^",			_XOR , 					) \
X("?",			_TERNARY, 				) \
X(":",		 	_COLON, 				) \
X("{",		 	_OPEN_BRACKETS, 		) \
X("}",		 	_CLOSE_BRACKETS, 		) \
X("("		 	_OPEN_PARENTHESIS, 		) \
X(")",		 	_CLOSE_PARENTHESIS, 	) \
X("\"",			_QUOTATION_MARK, 		) \
X("'",		 	_APOSTROPHE, 			) \
X(";",		 	_SEMICOLON, 			) \
X(",",		 	_COMMA, 				) \
X(".",		 	_DOT, 					) \
X("[a-z]",		_IDENTIFIER				)

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

// Auxiliar definitions for commands X MACRO
#define X_CMD_EXPAND_TABLE(a,b)			{.regExp = "^" a, .type = b},
#define X_CMD_EXPAND_TYPE(a,b)			b,
#define X_CMD_EXPAND_NAMES(a,b)			#b,
#define X_CMD_EXPAND_COUNT(a,b)			+1
#define CMD_COUNT						(0 X_COMMANDS(X_CMD_EXPAND_COUNT))

//------------------------------------------------------------------------------
// USER TYPES
//------------------------------------------------------------------------------

enum CommandType {
  X_COMMANDS(X_CMD_EXPAND_TYPE)
};

struct Command {
  char *regExp;
  enum CommandType type;
};

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __COMMANDS
