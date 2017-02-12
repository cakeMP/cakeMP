#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>

#include <System/Entity.h>

#include <enet/enet.h>

NAMESPACE_BEGIN;

class NetworkManager
{
public:
	bool m_connected = false;

private:
	ENetHost* m_localHost = nullptr;
	ENetPeer* m_localPeer = nullptr;

	std::queue<NetworkMessage*> m_incomingMessages;
	std::queue<NetworkMessage*> m_outgoingMessages;

	std::unordered_map<uint32_t, Entity*> m_entitiesNetwork;

public:
	NetworkManager();
	~NetworkManager();

	void Connect(const char* hostname, uint16_t port);
	void Disconnect();

	void ClearEntities();

	void SendToHost(NetworkMessage* message);

	template<typename T>
	T* GetEntityFromHandle(const NetHandle &handle);

	void Initialize();
	void Update();
	void HandleMessage(NetworkMessage* message);
};

NAMESPACE_END;
