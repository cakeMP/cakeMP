#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

extern int ui_screenWidth;
extern int ui_screenHeight;

extern void uiAddLongString(const char* str);

extern void uiNotify(const char* str);
extern void uiNotify(const std::string &str);

NAMESPACE_END;
