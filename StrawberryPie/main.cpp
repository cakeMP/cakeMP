#include <Common.h>
#include <Utils.h>

#include <Windows.h>

#include <shv/main.h>
#include <shv/natives.h>

static void appScriptMain()
{
	logWrite("Main script running");

	while (true) {
		Player player = PLAYER::PLAYER_ID();
		Ped ped = PLAYER::GET_PLAYER_PED(player);

		PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(player, 0.0f);
		PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);

		if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0)
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, false);

		VEHICLE::SET_RANDOM_BOATS(false);
		VEHICLE::SET_RANDOM_TRAINS(false);
		VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		VEHICLE::SET_NUMBER_OF_PARKED_VEHICLES(-1);
		VEHICLE::SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(false);
		VEHICLE::SET_FAR_DRAW_VEHICLES(false);

		MOBILE::DESTROY_MOBILE_PHONE();

		PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);

		CONTROLS::DISABLE_CONTROL_ACTION(0, 243, true); // EnterCheatCode
		CONTROLS::DISABLE_CONTROL_ACTION(0, 199, true); // FrontEndPause
		CONTROLS::DISABLE_CONTROL_ACTION(0, 200, true); // FrontEndPauseAlternate
		CONTROLS::DISABLE_CONTROL_ACTION(0, 212, true); // FrontEndSocialClub
		CONTROLS::DISABLE_CONTROL_ACTION(0, 213, true); // FrontEndSocialClubSecondary

		UI::HIDE_HELP_TEXT_THIS_FRAME();
		UI::DISPLAY_CASH(false);

		DECISIONEVENT::SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME();
		DECISIONEVENT::SUPPRESS_AGITATION_EVENTS_NEXT_FRAME();

		if (GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS()) {
			GAMEPLAY::CANCEL_STUNT_JUMP();
		}

		WAIT(0);
	}
}

static void appKeyboardHandler(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key == VK_F3 && !wasDownBefore && !isUpNow) {
		logWrite("F3 was pressed");
	}
}

static void appInitialize(HMODULE hInstance)
{
	logOpen(PROJECT_NAME_SHORT ".log");
	logWrite("Initializing v" PROJECT_VERSION);

	scriptRegister(hInstance, appScriptMain);
	keyboardHandlerRegister(appKeyboardHandler);
}

static void appUninitialize(HMODULE hInstance)
{
	keyboardHandlerUnregister(appKeyboardHandler);
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
