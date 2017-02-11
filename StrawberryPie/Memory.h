#pragma once

#pragma warning(disable: 4291) // no matching operator delete found

extern void* memAlloc(size_t size, const char* fnm, int line);
extern void* memCalloc(size_t size, const char* fnm, int line);
extern void* memRealloc(void* p, size_t size, const char* fnm, int line);
extern void memFree(void* p, const char* fnm, int line);

extern void* operator new(size_t size, const char* fnm, int line) throw();
extern void* operator new[](size_t size, const char* fnm, int line) throw();

extern void memTest(bool freeMemory = false);

#define malloc(size) memAlloc(size, __FILE__, __LINE__)
#define calloc(size) memCalloc(size, __FILE__, __LINE__)
#define realloc(p, size) memRealloc(p, size, __FILE__, __LINE__)
#define free(p) memFree(p, __FILE__, __LINE__)

#define new new(__FILE__, __LINE__)
