#include <Common.h>

#include <GameServer.h>

static volatile bool g_running = true;

#ifndef _MSC_VER
#include <signal.h>

static void IntHandler(int dummy)
{
	g_running = false;
}
#endif

GameServer* _pServer = nullptr;

GameServer::GameServer()
	: m_network(this)
{
}

GameServer::~GameServer()
{
}

void GameServer::Run()
{
#ifndef _MSC_VER
	signal(SIGINT, IntHandler);
#endif

	logWrite("Server starting...");

	m_tmStarted = Clock::now();

	m_network.Listen(m_settings.ListenHost.c_str(), m_settings.ListenPort, m_settings.MaxClients);

	while (m_running && g_running) {
		Update();

#ifdef _MSC_VER
		Sleep(1000 / m_settings.TickRate);
#else
		usleep((1000 / m_settings.TickRate) * 1000);
#endif
	}

	logWrite("Server shutting down...");
}

void GameServer::Update()
{
	m_world.Update();
	m_network.Update();
}
