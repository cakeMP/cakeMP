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

	while (true) {
		_pGame->Update();
		WAIT(0);
	}
}
