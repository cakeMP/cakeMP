#include <Common.h>
#include <Scripts/Game.h>

#include <System/Strawberry.h>

#include <shv/main.h>
#include <shv/natives.h>

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

	while (true) {
		_pGame->Update();

		WAIT(0);
	}
}
