#include "GameServer.h"

int main()
{
	_pServer = new GameServer();
	_pServer->Run();
	delete _pServer;

	return 0;
}
