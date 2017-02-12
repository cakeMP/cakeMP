#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>

#include <enet/enet.h>

class NetworkManager
{
public:
	bool m_connected = false;

private:
	ENetHost* m_localHost = nullptr;
	ENetPeer* m_localPeer = nullptr;

	std::queue<NetworkMessage*> m_incomingMessages;
	std::queue<NetworkMessage*> m_outgoingMessages;

public:
	NetworkManager();
	~NetworkManager();

	void Connect(const char* hostname, uint16_t port);
	void Disconnect();

	void SendToHost(NetworkMessage* message);

	void Initialize();
	void Update();
};
