#include <Common.h>
#include <Utils.h>

#include <Windows.h>

#include <shv/main.h>

static void appScriptMain()
{
	logWrite("Main script running");

	while (true) {
		WAIT(1000);
		logWrite("Tick...");
	}
}

static void appInitialize(HMODULE hInstance)
{
	logOpen(PROJECT_NAME_SHORT ".log");
	logWrite("Initializing v" PROJECT_VERSION);

	scriptRegister(hInstance, appScriptMain);
}

static void appUninitialize(HMODULE hInstance)
{
	scriptUnregister(hInstance);

	logWrite("Uninitializing");
	logClose();
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		appInitialize(hInstance);
	} else if (reason == DLL_PROCESS_DETACH) {
		appUninitialize(hInstance);
	}
	return TRUE;
}
