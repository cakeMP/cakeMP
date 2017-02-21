#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

extern void logOpen(const char* fnm);
extern void logClose();
extern void logWrite(const char* str, ...);
extern void logAssertFailed(const char* condition, const char* filename, int line);

extern bool mdlRequest(uint32_t hash);
extern bool mdlRequest(const char* str);

extern void sndPlayFrontend(const char* name, const char* set);
extern void sndPlayFrontend(const char* name);

NAMESPACE_END;
