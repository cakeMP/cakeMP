#pragma once

#include <Common.h>

#include <Network/NetworkMessage.h>
#include <Network/NetHandle.h>

#include <enet/enet.h>

class Player
{
private:
	ENetPeer* m_peer = nullptr;
	bool m_orderedToDisconnect = false;

	ClockTime m_tmSyncLastPosition;

public:
	NetHandle m_handle; //TODO: Move this into some abstract NetworkEntity or SyncedEntity class

	std::string m_nickname;
	std::string m_username;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

public:
	Player(ENetPeer* peer);
	~Player();

private:
	// Don't use the copyconstructor!!
	Player(const Player &player) {}

public:
	void OnConnected();
	void OnDisconnected();

	void Kick(const std::string &reason);
	void Close();

	void HandleMessage(NetworkMessage* message);

	void Update();

	//TODO: Make these getters atomic
	inline ENetPeer* GetPeer() { return m_peer; }
};
