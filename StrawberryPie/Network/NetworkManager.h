#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>

#include <System/Entity.h>

#include <enet/enet.h>

NAMESPACE_BEGIN;

class NetworkManager
{
private:
	bool m_connected = false;

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

	bool IsConnecting();
	bool IsConnected();

	void ClearEntities();

	void SendToHost(NetworkMessage* message);

	template<typename T>
	T* GetEntityFromHandle(NetworkEntityType expectedType, const NetHandle &handle);

	void Initialize();
	void Update();
	void HandleMessage(NetworkMessage* message);
};

NAMESPACE_END;
