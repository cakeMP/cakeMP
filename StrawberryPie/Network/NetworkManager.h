#pragma once

#include <enet/enet.h>

class NetworkManager
{
private:
	ENetHost* m_localHost = nullptr;
	ENetPeer* m_localPeer = nullptr;

public:
	NetworkManager();
	~NetworkManager();

	void Connect(const char* hostname, uint16_t port);
	void Disconnect();

	void Initialize();
	void Update();
};
