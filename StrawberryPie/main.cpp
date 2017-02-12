#include <Common.h>

#include <System/Strawberry.h>

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

static void appInitialize(HMODULE hInstance)
{
	logOpen(PROJECT_NAME_SHORT ".log");
	logWrite("Initializing v" PROJECT_VERSION);

	_pGame = new Strawberry(hInstance);

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
