#pragma once

extern void logOpen(const char* fnm);
extern void logClose();
extern void logWrite(const char* str, ...);
extern void logAssertFailed(const char* condition, const char* filename, int line);
