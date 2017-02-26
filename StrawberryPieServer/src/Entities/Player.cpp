#include <Common.h>

#include <Entities/Player.h>
#include <GameServer.h>

#include <Network/NetworkEntityType.h>
#include <Network/Structs/CreatePed.h>

Player::Player(ENetPeer* peer, const NetHandle &handle)
	: Entity(handle)
{
	m_peer = peer;

	//TODO: Make spawn position configurable (or something)
	m_position.x = -0.341730f;
	m_position.y = 525.319763f;
	m_position.z = 179.050201;

	m_model = hashGet("a_f_y_bevhills_02");
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
	NetworkMessage* msgLeave = new NetworkMessage(NMT_PlayerLeave);
	msgLeave->Write(m_handle);
	_pServer->m_network.SendMessageToAll(msgLeave);

	m_peer = nullptr;
}

void Player::Kick(const std::string &reason)
{
	if (m_peer == nullptr) {
		return;
	}

	if (m_orderedToDisconnect) {
		logWrite("Peer %08X was forced to disconnect", m_peer->address.host);
		enet_peer_disconnect(m_peer, 0);
		m_peer = nullptr;
		return;
	}

	logWrite("Kicking %08X because '%s'", m_peer->address.host, reason.c_str());

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

		logWrite("Player info received: %s (nickname: %s)", m_username.c_str(), m_nickname.c_str());

		// Send back player handshake
		NetworkMessage* msgHandshake = new NetworkMessage(NMT_Handshake);
		msgHandshake->Write(m_handle);
		msgHandshake->Write(m_position);
		msgHandshake->Write(m_model);
		_pServer->m_network.SendMessageTo(m_peer, msgHandshake);

		// See if we have to stream any entities in
		CheckStreamingEntities();
		//TODO: This should be part of streaming code, we shouldn't send the entire world
		/*
		NetworkMessage* msgCreateEntities = new NetworkMessage(NMT_CreateEntities);
		//TODO: This looks unsafe; we assume there's always X entities - 1 (the current player)
		uint32_t numEntities = _pServer->m_world.m_allEntities.size() - 1;
		msgCreateEntities->Write(numEntities);
		for (auto &pair : _pServer->m_world.m_allEntities) {
			if (pair.second == this) {
				continue;
			}
			pair.second->NetworkSerialize(msgCreateEntities);
		}
		_pServer->m_network.SendMessageTo(m_peer, msgCreateEntities);
		*/

		// Tell everyone else we joined
		//TODO: This shouldn't create a ped on the client! We have to use streaming for this.
		NetworkMessage* msgJoin = new NetworkMessage(NMT_PlayerJoin);
		msgJoin->Write(GetNetworkCreatePedStruct());
		msgJoin->Write(m_username);
		msgJoin->Write(m_nickname);
		_pServer->m_network.SendMessageToAll(msgJoin, m_peer);

		return;
	}

	if (message->m_type == NMT_ChatMessage) {
		std::string chatMessage;
		message->Read(chatMessage);

		logWrite("%s: %s", m_username.c_str(), chatMessage.c_str());

		NetworkMessage* msgChat = new NetworkMessage(NMT_ChatMessage);
		msgChat->Write(m_handle);
		msgChat->Write(chatMessage);
		_pServer->m_network.SendMessageToAll(msgChat, m_peer);

		return;
	}

	if (message->m_type == NMT_PlayerMove) {
		glm::vec3 newPosition, newVelocity;
		float newHeading;
		uint8_t newMoveType;

		message->Read(newPosition);
		message->Read(newHeading);
		message->Read(newVelocity);
		message->Read(newMoveType);

		//TODO: Process vectors for validity (maybe even do NaN checks for floats and vectors in NetworkMessage::Read())

		m_position = newPosition;
		m_rotation.z = newHeading;
		m_velocity = newVelocity;
		m_moveType = newMoveType;

		return;
	}
}

void Player::CheckStreamingEntities()
{
	std::vector<Entity*> newStreaming;
	_pServer->m_world.QueryRange(m_position, _pServer->m_settings.StreamingRange, newStreaming);

	std::vector<Entity*> streamedIn;
	std::vector<Entity*> streamedOut;

	auto itOld = m_streamedEntities.begin();
	auto itNew = newStreaming.begin();

	while (itOld != m_streamedEntities.end() && itNew != newStreaming.end()) {
		if (*itOld < *itNew) {
			streamedOut.push_back(*itOld++);
		} else if (*itNew < *itOld) {
			streamedIn.push_back(*itNew++);
		} else {
			itOld++;
			itNew++;
		}
	}

	while (itOld != m_streamedEntities.end()) {
		streamedOut.push_back(*itOld++);
	}
	while (itNew != newStreaming.end()) {
		streamedIn.push_back(*itNew++);
	}

	m_streamedEntities = newStreaming;

	if (streamedIn.size() > 0) {
		NetworkMessage* msgStreamIn = new NetworkMessage(NMT_StreamIn);
		msgStreamIn->Write((uint32_t)streamedIn.size());
		for (Entity* ent : streamedIn) {
			ent->NetworkSerialize(msgStreamIn);
		}
		_pServer->m_network.SendMessageTo(m_peer, msgStreamIn);
	}

	if (streamedOut.size() > 0) {
		NetworkMessage* msgStreamOut = new NetworkMessage(NMT_StreamOut);
		msgStreamOut->Write((uint32_t)streamedOut.size());
		for (Entity* ent : streamedOut) {
			msgStreamOut->Write(ent->m_handle);
		}
		_pServer->m_network.SendMessageTo(m_peer, msgStreamOut);
	}
}

NetStructs::CreatePed Player::GetNetworkCreatePedStruct()
{
	NetStructs::CreatePed ret;
	ret.m_handle = m_handle;
	ret.m_model = m_model;
	ret.m_position = m_position;
	ret.m_rotation = m_rotation;
	return ret;
}

void Player::NetworkSerialize(NetworkMessage* message)
{
	message->Write(ET_Player);
	message->Write(GetNetworkCreatePedStruct());
	message->Write(m_username);
	message->Write(m_nickname);
}

void Player::Update()
{
	if ((int)ClockDuration(Clock::now() - m_tmSyncLastPosition).count() > 250) {
		m_tmSyncLastPosition = Clock::now();

		NetworkMessage* msgPos = new NetworkMessage(NMT_PlayerMove);
		//TODO: Turn this into a NetStruct
		msgPos->Write(m_handle);
		msgPos->Write(m_position);
		msgPos->Write(m_rotation.z);
		msgPos->Write(m_velocity);
		msgPos->Write(m_moveType);
		_pServer->m_network.SendMessageToRange(m_position, _pServer->m_settings.StreamingRange, msgPos, m_peer);
	}

	Entity::Update();
}
