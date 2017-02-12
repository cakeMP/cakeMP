#include <Common.h>

#include <Network/Player.h>
#include <GameServer.h>

Player::Player(ENetPeer* peer)
{
	m_peer = peer;
}

Player::~Player()
{
	if (m_peer != nullptr) {
		Close();
	}
}

void Player::OnConnected()
{
	Kick("Hey this is my server, fuck off");
}

void Player::OnDisconnected()
{
	m_peer = nullptr;
}

void Player::Kick(const std::string &reason)
{
	if (m_peer == nullptr) {
		return;
	}

	if (m_orderedToDisconnect) {
		printf("Peer %08X was forced to disconnect\n", m_peer->address.host);
		enet_peer_disconnect(m_peer, 0);
		m_peer = nullptr;
		return;
	}

	printf("Kicking %08X because '%s'\n", m_peer->address.host, reason.c_str());

	NetworkMessage* msgDisconnect = new NetworkMessage(NMT_Disconnect);
	msgDisconnect->Write(reason);
	_pServer->m_network.SendMessageTo(m_peer, msgDisconnect);

	m_orderedToDisconnect = true;
}

void Player::Close()
{
	if (m_peer == nullptr) {
		return;
	}

	enet_peer_disconnect_now(m_peer, 0);
	m_peer = nullptr;
}

void Player::Update()
{
}
