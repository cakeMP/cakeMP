#pragma once

#include <Common.h>

#include <World/Entity.h>

#include <Network/NetworkMessage.h>
#include <Network/NetHandle.h>
#include <Network/Structs/CreatePed.h>

#include <enet/enet.h>

class Player : public Entity
{
private:
	ENetPeer* m_peer = nullptr;
	bool m_orderedToDisconnect = false;

	ClockTime m_tmSyncLastPosition;
	ClockTime m_tmLastStreaming;

	std::vector<Entity*> m_streamedEntities;

public:
	uint32_t m_model = 0;

	std::string m_nickname;
	std::string m_username;

public:
	Player(ENetPeer* peer, const NetHandle &handle);
	virtual ~Player();

	void OnConnected();
	void OnDisconnected();

	void Kick(const std::string &reason);
	void Close();

	void HandleMessage(NetworkMessage* message);

	void CheckStreamingEntities();

	virtual NetworkEntityType GetType();

	NetStructs::CreatePed GetNetworkCreatePedStruct();
	virtual void NetworkSerialize(NetworkMessage* message);

	virtual void Update();

	//TODO: Make these getters atomic
	inline ENetPeer* GetPeer() { return m_peer; }
};
