#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>
#include <Network/Player.h>

#include <enet/enet.h>

class NetworkManager
{
private:
	ENetHost* m_hostListen = nullptr;

	std::queue<NetworkMessage*> m_incomingMessages;
	std::queue<NetworkMessage*> m_outgoingMessages;

public:// THIS IS PUBLIC JUST FOR TESTING
	std::vector<Player*> m_players;

public:
	NetworkManager();
	~NetworkManager();

	Player* FindPlayer(const ENetPeer* peer);

	void Listen(const char* host, uint16_t port, uint32_t maxClients);
	void Close();

	void SendMessageTo(ENetPeer* peer, NetworkMessage* message);

	void Update();
};
