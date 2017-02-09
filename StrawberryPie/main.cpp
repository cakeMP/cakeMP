#include <Common.h>
#include <Utils.h>

#include <Windows.h>

#include <shv/main.h>
#include <shv/natives.h>

static void appScriptMain()
{
	logWrite("Main script running");

	while (true) {
		VEHICLE::SET_RANDOM_TRAINS(false);
		VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		VEHICLE::SET_NUMBER_OF_PARKED_VEHICLES(-1);
		VEHICLE::SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(false);
		VEHICLE::SET_FAR_DRAW_VEHICLES(false);

		MOBILE::DESTROY_MOBILE_PHONE();

		PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);

		UI::DISPLAY_CASH(false);

		WAIT(0);
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
