#pragma once

extern void logWrite(const char* fmt, ...);

#define TC_RESET    "\033[0m"
#define TC_BLACK    "\033[30m"
#define TC_RED      "\033[31m"
#define TC_GREEN    "\033[32m"
#define TC_YELLOW   "\033[33m"
#define TC_BLUE     "\033[34m"
#define TC_MAGENTA  "\033[35m"
#define TC_CYAN     "\033[36m"
#define TC_WHITE    "\033[37m"
#define TC_BBLACK   "\033[1m\033[30m"
#define TC_BRED     "\033[1m\033[31m"
#define TC_BGREEN   "\033[1m\033[32m"
#define TC_BYELLOW  "\033[1m\033[33m"
#define TC_BBLUE    "\033[1m\033[34m"
#define TC_BMAGENTA "\033[1m\033[35m"
#define TC_BCYAN    "\033[1m\033[36m"
#define TC_BWHITE   "\033[1m\033[37m"
