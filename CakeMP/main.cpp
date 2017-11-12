#include <Common.h>

#include <System/Cake.h>

#include <Utils/AppMem.h>

#include <Build.h>

#include <Windows.h>

#include <shv/main.h>
#include <shv/natives.h>

NAMESPACE_BEGIN;

static void appKeyboardHandler(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (!wasDownBefore && !isUpNow) {
		_pGame->OnKeyDown(key);
	} else if (wasDownBefore && isUpNow) {
		_pGame->OnKeyUp(key);
	}
}

struct NativeReg
{
	NativeReg* next;
	void* handlers[7];
	uint32_t numEntries;
	uint64_t hashes[7];
};

static void appInitialize(HMODULE hInstance)
{
	logOpen(PROJECT_NAME_SHORT ".log");
	logWrite("Initializing v" PROJECT_VERSION);

	uint8_t* pMapNatives = memFindPattern("48 89 5C 24 ?? 48 89 7C 24 ?? 45 33 C0 4C");
	if (pMapNatives != nullptr) {
		logWrite("Dumping natives...");

		uint32_t offset = *(uint32_t*)(pMapNatives + 0x1D + 3);
		NativeReg** table = (NativeReg**)(pMapNatives + 0x24 + offset);

		int numNatives = 0;

		FILE* fh = fopen("natives.txt", "wb");
		if (fh != nullptr) {
			int tableIndex = 0;
			NativeReg* cur = cur = table[tableIndex];
			do {
				while (cur != nullptr) {
					for (uint32_t i = 0; i < cur->numEntries; i++) {
						fprintf(fh, "0x%016llX @ %p\n", cur->hashes[i], cur->handlers[i]);
						numNatives++;
					}
					cur = cur->next;
				}
				cur = table[++tableIndex];
			} while (cur != nullptr);
			fclose(fh);
		}

		logWrite("Done!");
	}

	_pGame = new Cake(hInstance);

	keyboardHandlerRegister(appKeyboardHandler);
}

static void appUninitialize()
{
	logWrite("Uninitializing");

	keyboardHandlerUnregister(appKeyboardHandler);

	delete _pGame;

	memTest();
	logClose();
}

NAMESPACE_END;

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		NAMESPACE_NAME::appInitialize(hInstance);
	} else if (reason == DLL_PROCESS_DETACH) {
		NAMESPACE_NAME::appUninitialize();
	}
	return TRUE;
}
