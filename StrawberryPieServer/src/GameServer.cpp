#include <Common.h>

#include <GameServer.h>

GameServer* _pServer = nullptr;

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}

void GameServer::Run()
{
	printf("Server starting...\n");

	m_network.Listen(m_settings.ListenHost.c_str(), m_settings.ListenPort, m_settings.MaxClients);

	while (m_running) {
		Update();

		usleep((1000 / m_settings.TickRate) * 1000);
	}

	printf("Server shutting down...\n");
}

void GameServer::Update()
{
	m_network.Update();
}
