#pragma once

#include <Common.h>

#include <enet/enet.h>

class NetworkManager
{
public:
	ENetHost* m_hostListen = nullptr;

public:
	NetworkManager();
	~NetworkManager();

	void Listen(const char* host, uint16_t port, uint32_t maxClients);
	void Close();

	void Update();
};
