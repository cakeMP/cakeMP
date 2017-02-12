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

void Player::HandleMessage(NetworkMessage* message)
{
	if (message->m_type == NMT_Handshake) {
		std::string username, nickname;

		message->Read(username);
		message->Read(nickname);

		//TODO: Process handshake data for validity

		m_username = username;
		m_nickname = nickname;

		printf("Player info received: %s (nickname: %s)\n", m_username.c_str(), m_nickname.c_str());

		return;
	}

	if (message->m_type == NMT_ChatMessage) {
		std::string chatMessage;
		message->Read(chatMessage);

		printf("%s: %s\n", "Player", m_username.c_str(), chatMessage.c_str());

		return;
	}

	if (message->m_type == NMT_PlayerMove) {
		glm::vec3 newPosition, newRotation;

		message->Read(newPosition);
		message->Read(newRotation);

		//TODO: Process vectors for validity (maybe even do NaN checks for floats and vectors in NetworkMessage::Read())

		m_position = newPosition;
		m_rotation = newRotation;

		return;
	}
}

void Player::Update()
{
	if ((int)ClockDuration(Clock::now() - m_tmSyncLastPosition).count() > 100) {
		m_tmSyncLastPosition = Clock::now();

		NetworkMessage* msgPos = new NetworkMessage(NMT_PlayerMove);
		msgPos->Write(m_handle);
		msgPos->Write(m_position);
		msgPos->Write(m_rotation);
		_pServer->m_network.SendMessageToAll(msgPos, GetPeer());
	}
}
