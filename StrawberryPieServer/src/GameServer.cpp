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
	logWrite("Server starting...\n");

	m_tmStarted = Clock::now();

	m_network.Listen(m_settings.ListenHost.c_str(), m_settings.ListenPort, m_settings.MaxClients);

	while (m_running) {
		Update();

		usleep((1000 / m_settings.TickRate) * 1000);
	}

	logWrite("Server shutting down...\n");
}

void GameServer::Update()
{
	m_network.Update();
}
