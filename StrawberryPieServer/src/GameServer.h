#pragma once

#include <Network/NetworkManager.h>
#include <Settings.h>

#include <World.h>

class GameServer
{
public:
	NetworkManager m_network;
	Settings m_settings;
	World m_world;

	bool m_running = true;

	ClockTime m_tmStarted;

public:
	GameServer();
	~GameServer();

	void Run();

	void Update();
};

extern GameServer* _pServer;
