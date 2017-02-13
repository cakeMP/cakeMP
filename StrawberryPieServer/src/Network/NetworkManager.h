#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>
#include <Network/NetHandle.h>
#include <Entities/Player.h>

#include <enet/enet.h>

class NetworkManager
{
private:
	ENetHost* m_hostListen = nullptr;

	std::queue<NetworkMessage*> m_incomingMessages;
	std::queue<NetworkMessage*> m_outgoingMessages;

	std::vector<Player*> m_players;

	uint32_t m_handleIterator = 1;

public: //TODO: Maybe this shouldn't be public
	std::unordered_map<uint32_t, Entity*> m_entities;

public:
	NetworkManager();
	~NetworkManager();

	NetHandle AssignHandle();

	void Listen(const char* host, uint16_t port, uint32_t maxClients);
	void Close();

	void SendMessageTo(ENetPeer* peer, NetworkMessage* message);
	void SendMessageToAll(NetworkMessage* message);
	void SendMessageToAll(NetworkMessage* message, ENetPeer* except);

	void Update();
};
