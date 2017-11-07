#include <Common.h>

#include <Utils/AppMem.h>
#include <Utils/Logging.h>

#include <Windows.h>
#include <TlHelp32.h>

NAMESPACE_BEGIN;

uint8_t* memGetBasePointer()
{
	MODULEENTRY32 me32;
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	me32.dwSize = sizeof(MODULEENTRY32);

	void* ret = nullptr;
	if (Module32First(hModule, &me32)) {
		ret = me32.modBaseAddr;
	}

	CloseHandle(hModule);

	return (uint8_t*)ret;
}

uint8_t* memFindPattern(uint8_t* buffer, size_t size, const char* pattern)
{
	PatternByte* compiled = nullptr;

	int numBytes = ((int)strlen(pattern) + 1) / 3;
	if (numBytes == 0) {
		return nullptr;
	}

	compiled = (PatternByte*)malloc(numBytes * sizeof(PatternByte));

	int byteIndex = 0;

	const char* p = pattern;
	while (*p != '\0') {
		PatternByte &byte = compiled[byteIndex];

		if (*p == '?') {
			byte.mask = true;

			p += 3;
			byteIndex++;
			continue;
		}

		sscanf(p, "%hhx", &byte.byte);
		byte.mask = false;

		byteIndex++;
		p += 3;
	}

	if (byteIndex == 0) {
		free(compiled);
		return nullptr;
	}

	size_t startOffset = 0;
	size_t cur = 0;

	for (size_t offset = 0; offset < size; offset++) {
		PatternByte &byte = compiled[cur];
		if (byte.mask) {
			cur++;
		} else if (buffer[offset] == byte.byte) {
			if (cur == 0) {
				startOffset = offset;
			}
			if (++cur == numBytes) {
				free(compiled);
				return buffer + startOffset;
			}
		} else {
			if (cur > 0) {
				cur = 0;
				offset = startOffset;
			}
		}
	}

	free(compiled);
	return nullptr;
}

uint8_t* memFindPattern(const char* pattern)
{
	const char* findModule = "GTA5.exe";

	MODULEENTRY32 me32;
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hModule, &me32)) {
		if (strcmp(me32.szModule, findModule) != 0) {
			while (Module32Next(hModule, &me32)) {
				if (!strcmp(me32.szModule, findModule)) {
					break;
				}
			}
		}
	}

	if (strcmp(me32.szModule, findModule) != 0) {
		logWrite("Unable to find module '%s'!", findModule);
		return nullptr;
	}

	uint8_t* ret = memFindPattern(me32.modBaseAddr, me32.modBaseSize, pattern);

	CloseHandle(hModule);

	return ret;
}

NAMESPACE_END;
