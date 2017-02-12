#pragma once

#include <Network/NetworkManager.h>
#include <Settings.h>

class GameServer
{
public:
	NetworkManager m_network;

	Settings m_settings;

	bool m_running = true;

	ClockTime m_tmStarted;

public:
	GameServer();
	~GameServer();

	void Run();

	void Update();
};

extern GameServer* _pServer;
