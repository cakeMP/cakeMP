#include <Common.h>
#include <Scripts/Game.h>

#include <System/Strawberry.h>

#include <shv/main.h>
#include <shv/natives.h>

NAMESPACE_BEGIN;

void scriptGame()
{
	while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) {
		WAIT(0);
	}

	_pGame->Initialize();

	/*
	UI::_SET_LOADING_PROMPT_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Loading " PROJECT_NAME);
	UI::_SHOW_LOADING_PROMPT(3);
	WAIT(1000);
	UI::_REMOVE_LOADING_PROMPT();
	*/

	const int UpdateInterval = (int)((1000 / 60.0f) * 1000);

	ClockTime tmLastUpdate = Clock::now();

	while (true) {
		ClockTime tmNow = Clock::now();
		auto tmSinceLastUpdate = MicroDuration(tmNow - tmLastUpdate);

		if (tmSinceLastUpdate.count() >= UpdateInterval) {
			tmLastUpdate = tmNow;
			_pGame->Update(tmSinceLastUpdate.count() / 1000.0f);
		}
		_pGame->Frame();

		WAIT(0);
	}
}

NAMESPACE_END;
