#include <Common.h>

#include <Strawberry.h>
#include <Utils.h>

#include <Windows.h>

#include <shv/main.h>
#include <shv/natives.h>

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
	keyboardHandlerUnregister(appKeyboardHandler);

	delete _pGame;

	logWrite("Uninitializing");
	logClose();
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		appInitialize(hInstance);
	} else if (reason == DLL_PROCESS_DETACH) {
		appUninitialize();
	}
	return TRUE;
}
