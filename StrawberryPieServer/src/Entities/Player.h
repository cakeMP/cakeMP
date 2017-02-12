#pragma once

#include <Common.h>

#include <Entity.h>

#include <Network/NetworkMessage.h>
#include <Network/NetHandle.h>

#include <enet/enet.h>

class Player : public Entity
{
private:
	ENetPeer* m_peer = nullptr;
	bool m_orderedToDisconnect = false;

	ClockTime m_tmSyncLastPosition;

public:
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

	virtual void Update();

	//TODO: Make these getters atomic
	inline ENetPeer* GetPeer() { return m_peer; }
};
