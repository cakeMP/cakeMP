#include <Common.h>

#include <Entities/Player.h>
#include <GameServer.h>

Player::Player(ENetPeer* peer, const NetHandle &handle)
	: Entity(handle)
{
	m_peer = peer;

	//TODO: Make spawn position configurable (or something)
	m_position.x = -0.341730f;
	m_position.y = 525.319763f;
	m_position.z = 179.050201;
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

		NetworkMessage* msgHandshake = new NetworkMessage(NMT_Handshake);
		msgHandshake->Write(m_handle);
		msgHandshake->Write(m_position);
		_pServer->m_network.SendMessageTo(m_peer, msgHandshake);

		NetworkMessage* msgJoin = new NetworkMessage(NMT_PlayerJoin);
		msgJoin->Write(m_handle);
		msgJoin->Write(m_username);
		msgJoin->Write(m_nickname);
		msgJoin->Write(m_position);
		_pServer->m_network.SendMessageToAll(msgJoin, m_peer);

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
