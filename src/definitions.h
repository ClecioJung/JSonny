#ifndef __DEFINITIONS
#define __DEFINITIONS

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------

// Software version
#define __VERSION								"0.0.2"

// Font definitions
#define RESET_FONT							"\033[0m"
#define BOLD_FONT								"\033[1m"
#define FAINT_FONT							"\033[2m"
#define ITALIC_FONT_FONT					"\033[3m"
#define UNDERLINE_FONT						"\033[4m"

// Foreground color definitions
#define BLACK_FOREGROUND					"\033[30m"
#define RED_FOREGROUND						"\033[31m"
#define GREEN_FOREGROUND					"\033[32m"
#define YELLOW_FOREGROUND					"\033[33m"
#define BLUE_FOREGROUND						"\033[34m"
#define MAGENTA_FOREGROUND					"\033[35m"
#define CYAN_FOREGROUND						"\033[36m"
#define WHITE_FOREGROUND					"\033[37m"

#define BRIGHT_BLACK_FOREGROUND			"\033[90m"
#define BRIGHT_RED_FOREGROUND				"\033[91m"
#define BRIGHT_GREEN_FOREGROUND			"\033[92m"
#define BRIGHT_YELLOW_FOREGROUND			"\033[93m"
#define BRIGHT_BLUE_FOREGROUND			"\033[94m"
#define BRIGHT_MAGENTA_FOREGROUND		"\033[95m"
#define BRIGHT_CYAN_FOREGROUND			"\033[96m"
#define BRIGHT_WHITE_FOREGROUND			"\033[97m"

// Background color definitions
#define BLACK_BACKGROUND					"\033[40m"
#define RED_BACKGROUND						"\033[41m"
#define GREEN_BACKGROUND					"\033[42m"
#define YELLOW_BACKGROUND					"\033[43m"
#define BLUE_BACKGROUND						"\033[44m"
#define MAGENTA_BACKGROUND					"\033[45m"
#define CYAN_BACKGROUND						"\033[46m"
#define WHITE_BACKGROUND					"\033[47m"

#define BRIGHT_BLACK_BACKGROUND			"\033[100m"
#define BRIGHT_RED_BACKGROUND				"\033[101m"
#define BRIGHT_GREEN_BACKGROUND			"\033[102m"
#define BRIGHT_YELLOW_BACKGROUND			"\033[103m"
#define BRIGHT_BLUE_BACKGROUND			"\033[104m"
#define BRIGHT_MAGENTA_BACKGROUND		"\033[105m"
#define BRIGHT_CYAN_BACKGROUND			"\033[106m"
#define BRIGHT_WHITE_BACKGROUND			"\033[107m"

// Header to anounce to the user the meaning of current message
#define CRASH_HEADER							BOLD_FONT RED_FOREGROUND "[Crash] " RESET_FONT
#define ERROR_HEADER							BOLD_FONT RED_FOREGROUND "[Error] " RESET_FONT
#define WARNING_HEADER						BOLD_FONT MAGENTA_FOREGROUND "[Warning] " RESET_FONT
#define USAGE_HEADER							BOLD_FONT BLUE_FOREGROUND "[Usage] " RESET_FONT
#define VERSION_HEADER						BOLD_FONT BLUE_FOREGROUND "[Version] " RESET_FONT
#define ABOUT_HEADER              BOLD_FONT BLUE_FOREGROUND "[About] " RESET_FONT
#define SETTINGS_HEADER						BOLD_FONT BLUE_FOREGROUND "[Settings] " RESET_FONT

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif // __DEFINITIONS
