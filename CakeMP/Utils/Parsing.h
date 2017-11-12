#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

extern std::string prsToString(int32_t x);
extern std::string prsToString(uint32_t x);
extern std::string prsToString(int64_t x);
extern std::string prsToString(uint64_t x);
extern std::string prsToString(float x);

extern int prsToInteger(const char* str);
extern int prsToInteger(const std::string &str);

extern float prsToFloat(const char* str);
extern float prsToFloat(const std::string &str);

NAMESPACE_END;
