#include <Common.h>

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef new

#include <Utils.h>

#define MEMORY_DEBUG 1
#define MEMORY_PRINT logWrite
#define MEMORY_PRINT_NEWLINE

#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <vector>
#include <algorithm>
#include <mutex>

struct Allocation
{
	void* ptr;
	size_t size;

	const char* fnm;
	int line;
};

template<typename T>
struct DebugAllocator
{
	typedef T value_type;

	DebugAllocator() {}
	template<typename U> DebugAllocator(const DebugAllocator<U> &other) {}

	T* allocate(std::size_t n) { return (T*)malloc(n * sizeof(T)); }
	void deallocate(T* p, std::size_t n) { free(p); }
};

static std::vector<Allocation, DebugAllocator<Allocation>> _memAllocations;
static std::mutex _memMutex;

inline size_t memGetAllocIndex(void* p)
{
	return *(size_t*)((unsigned char*)p - sizeof(size_t));
}

inline Allocation &memGetAlloc(void* p)
{
	return _memAllocations[memGetAllocIndex(p)];
}

void* memAlloc(size_t size, const char* fnm, int line)
{
	unsigned char* ptr = (unsigned char*)malloc(size + sizeof(size_t));
	void* ret = ptr + sizeof(size_t);

	Allocation alloc;
	alloc.ptr = ptr;
	alloc.size = size;

	alloc.fnm = fnm;
	alloc.line = line;

	_memMutex.lock();

	_memAllocations.push_back(alloc);
	*(size_t*)ptr = _memAllocations.size() - 1;

#if MEMORY_DEBUG
	MEMORY_PRINT(".. memAlloc: %p at %s:%d" MEMORY_PRINT_NEWLINE, ret, fnm, line);
#endif

	_memMutex.unlock();

	return ret;
}

void* memCalloc(size_t size, const char* fnm, int line)
{
	void* ret = memAlloc(size, fnm, line);
	memset(ret, 0, size);

#if MEMORY_DEBUG
	MEMORY_PRINT(".. memCalloc: %p at %s:%d" MEMORY_PRINT_NEWLINE, ret, fnm, line);
#endif

	return ret;
}

void* memRealloc(void* p, size_t size, const char* fnm, int line)
{
	if (p == nullptr) {
		return memAlloc(size, fnm, line);
	}

	_memMutex.lock();

	Allocation &alloc = memGetAlloc(p);

	unsigned char* ptr = (unsigned char*)realloc(alloc.ptr, size + sizeof(size_t));
	void* ret = ptr + sizeof(size_t);

	alloc.ptr = ptr;
	alloc.size = size;

	alloc.fnm = fnm;
	alloc.line = line;

#if MEMORY_DEBUG
	MEMORY_PRINT(".. memRealloc: %p at %s:%d" MEMORY_PRINT_NEWLINE, ret, fnm, line);
#endif

	_memMutex.unlock();

	return ret;
}

void memFree(void* p, const char* fnm, int line)
{
	_memMutex.lock();

#if MEMORY_DEBUG
	MEMORY_PRINT(".. memFree: %p at %s:%d" MEMORY_PRINT_NEWLINE, p, fnm, line);
#endif

	size_t allocIndex = memGetAllocIndex(p);
	Allocation &alloc = memGetAlloc(p);
	free(alloc.ptr);
	_memAllocations.erase(_memAllocations.begin() + allocIndex);

	_memMutex.unlock();
}

void* operator new(size_t size, const char* fnm, int line) throw(){ return memAlloc(size, fnm, line); }
void* operator new[](size_t size, const char* fnm, int line) throw() { return memAlloc(size, fnm, line); }

void operator delete(void* p) { memFree(p, __FILE__, __LINE__); }
void operator delete(void* p, size_t size) { memFree(p, __FILE__, __LINE__); }

void operator delete[](void* p) { memFree(p, __FILE__, __LINE__); }
void operator delete[](void* p, size_t size) { memFree(p, __FILE__, __LINE__); }

void memTest(bool freeMemory)
{
	if (_memAllocations.size() > 0) {
		MEMORY_PRINT("== There are %zu unfreed memory allocations!" MEMORY_PRINT_NEWLINE, _memAllocations.size());
	}

	for (Allocation &alloc : _memAllocations) {
		MEMORY_PRINT("!! Memory leak of size %llu at %p found!" MEMORY_PRINT_NEWLINE, alloc.size, ((unsigned char*)alloc.ptr + sizeof(size_t)));
		MEMORY_PRINT("   Allocated at %s:%d" MEMORY_PRINT_NEWLINE, alloc.fnm, alloc.line);

		if (freeMemory) {
			memFree((unsigned char*)alloc.ptr + sizeof(size_t), __FILE__, __LINE__);
		}
	}
}
