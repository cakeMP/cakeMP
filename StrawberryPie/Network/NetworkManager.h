#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>

#include <System/Entity.h>

#include <Utils/Average.h>

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
	Average<uint32_t> m_statsIncomingMessages;
	Average<uint32_t> m_statsIncomingBytes;
	uint64_t m_statsIncomingMessagesTotal = 0;
	uint64_t m_statsIncomingBytesTotal = 0;
	Average<uint32_t> m_statsOutgoingMessages;
	Average<uint32_t> m_statsOutgoingBytes;
	uint64_t m_statsOutgoingMessagesTotal = 0;
	uint64_t m_statsOutgoingBytesTotal = 0;

public:
	NetworkManager();
	~NetworkManager();

	void Connect(const char* hostname, uint16_t port);
	void Disconnect();

	bool IsConnecting();
	bool IsConnected();

	void ClearEntities();
	int GetEntityCount();

	void SendToHost(NetworkMessage* message);

	Entity* GetEntityFromHandle(NetworkEntityType expectedType, const NetHandle &handle);
	Entity* GetEntityFromLocalHandle(NetworkEntityType expectedType, int handle);

	template<typename T>
	T* GetEntityFromHandle(NetworkEntityType expectedType, const NetHandle &handle)
	{
		return static_cast<T*>(GetEntityFromHandle(expectedType, handle));
	}

	template<typename T>
	T* GetEntityFromLocalHandle(NetworkEntityType expectedType, int handle)
	{
		return static_cast<T*>(GetEntityFromLocalHandle(expectedType, handle));
	}

	void Initialize();
	void Update();
	void HandleMessage(NetworkMessage* message);
};

NAMESPACE_END;
