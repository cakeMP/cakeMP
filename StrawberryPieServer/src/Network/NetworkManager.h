#pragma once

#include "..\Common.h"

#include <Network/NetworkMessage.h>
#include <Network/NetHandle.h>
#include "..\Entities\Player.h"

#include <enet/enet.h>

class GameServer;

class NetworkManager
{
private:
	GameServer* m_server = nullptr;

	ENetHost* m_hostListen = nullptr;

	std::queue<NetworkMessage*> m_incomingMessages;
	std::queue<NetworkMessage*> m_outgoingMessages;

	std::vector<Player*> m_players;

	uint32_t m_handleIterator = 1;

public:
	NetworkManager(GameServer* server);
	~NetworkManager();

	NetHandle AssignHandle();

	void Listen(const char* host, uint16_t port, uint32_t maxClients);
	void Close();

	//TODO: Deprecate these functions and have only one SendMessage() instead
	void SendMessageTo(ENetPeer* peer, NetworkMessage* message);
	void SendMessageToAll(NetworkMessage* message);
	void SendMessageToAll(NetworkMessage* message, ENetPeer* except);
	void SendMessageToRange(const glm::vec3 &pos, float range, NetworkMessage* message);
	void SendMessageToRange(const glm::vec3 &pos, float range, NetworkMessage* message, ENetPeer* except);

	void Update();
};
