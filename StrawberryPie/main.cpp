#include <cstdio>
#include <Windows.h>

#include <shv/main.h>

static void ScriptMain()
{
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		scriptRegister(hInstance, ScriptMain);
	} else if (reason == DLL_PROCESS_DETACH) {
		scriptUnregister(hInstance);
	}
}
