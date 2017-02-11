#pragma once

#include <Common.h>

#include <enet/enet.h>

class Player
{
private:
	ENetPeer* m_peer = nullptr;

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

	void Update();

	//TODO: Make these getters atomic
	inline ENetPeer* GetPeer() { return m_peer; }
};
