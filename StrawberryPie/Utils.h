#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

extern void logOpen(const char* fnm);
extern void logClose();
extern void logWrite(const char* str, ...);
extern void logAssertFailed(const char* condition, const char* filename, int line);

NAMESPACE_END;
