#include <Common.h>
#include <Scripts/Game.h>

#include <System/Strawberry.h>

#include <shv/main.h>

void scriptGame()
{
	while (true) {
		_pGame->Update();
		WAIT(0);
	}
}
