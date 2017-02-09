#pragma once

extern void logOpen(const char* fnm);
extern void logClose();
extern void logWrite(const char* str, ...);
