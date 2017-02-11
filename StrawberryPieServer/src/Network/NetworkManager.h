#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>

#include <enet/enet.h>

class NetworkManager
{
public:
	ENetHost* m_hostListen = nullptr;

	std::vector<NetworkMessage*> m_incomingMessages;
	std::vector<NetworkMessage*> m_outgoingMessages;

public:
	NetworkManager();
	~NetworkManager();

	void Listen(const char* host, uint16_t port, uint32_t maxClients);
	void Close();

	void SendMessageTo(uint32_t peer, NetworkMessage* message);

	void Update();
};
