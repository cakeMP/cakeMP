#include <Common.h>

#include <Entities/Player.h>
#include <Entities/Vehicle.h>
#include <GameServer.h>

#include <Network/Structs/CreatePed.h>

Player::Player(ENetPeer* peer, const NetHandle &handle)
	: Entity(handle)
{
	m_peer = peer;

	//TODO: Make spawn position configurable (or something)
	m_position.x = -0.341730f;
	m_position.y = 525.319763f;
	m_position.z = 179.050201f;

	m_model = hashGet("a_f_y_bevhills_02");

	m_tmSyncLastPosition = Clock::now();
	m_tmLastStreaming = Clock::now();
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

	for (Entity* ent : m_streamedEntities) {
		ent->Release();
	}
	m_streamedEntities.clear();
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

	for (Entity* ent : m_streamedEntities) {
		ent->Release();
	}
	m_streamedEntities.clear();

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

		// Give the client the current world state
		NetworkMessage* msgWeather = new NetworkMessage(NMT_Weather);
		msgWeather->Write("CLEAR");
		_pServer->m_network.SendMessageTo(m_peer, msgWeather);

		NetworkMessage* msgTime = new NetworkMessage(NMT_ClockTime);
		msgTime->Write(12);
		msgTime->Write(0);
		msgTime->Write(0);
		_pServer->m_network.SendMessageTo(m_peer, msgTime);

		// See if we have to stream any entities in
		CheckStreamingEntities();

		// Tell everyone else we joined
		NetworkMessage* msgJoin = new NetworkMessage(NMT_PlayerJoin);
		msgJoin->Write(m_handle);
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
		msgChat->Write(m_username + ": " + chatMessage);
		_pServer->m_network.SendMessageToAll(msgChat);

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

		SetPosition(newPosition);
		m_rotation.z = newHeading;
		m_velocity = newVelocity;
		m_moveType = newMoveType;

		return;
	}

	if (message->m_type == NMT_EnteringVehicle) {
		NetHandle vehicleHandle;
		int seat;

		message->Read(vehicleHandle);
		message->Read(seat);

		NetworkMessage* msgEnteringVehicle = new NetworkMessage(NMT_EnteringVehicle);
		msgEnteringVehicle->Write(m_handle);
		msgEnteringVehicle->Write(vehicleHandle);
		msgEnteringVehicle->Write(seat);
		_pServer->m_network.SendMessageToRange(m_position, _pServer->m_settings.StreamingRange, msgEnteringVehicle, m_peer);

		return;
	}

	if (message->m_type == NMT_EnteredVehicle) {
		NetHandle vehicleHandle;
		int seat;

		message->Read(vehicleHandle);
		message->Read(seat);

		Vehicle* vehicle = _pServer->m_world.GetEntityFromHandle<Vehicle>(vehicleHandle);
		if (vehicle == nullptr) {
			logWrite("Player tried entering unknown vehicle!");
			return;
		}

		vehicle->AddOccupant(this, seat);

		NetworkMessage* msgEnteredVehicle = new NetworkMessage(NMT_EnteredVehicle);
		msgEnteredVehicle->Write(m_handle);
		msgEnteredVehicle->Write(vehicle->m_handle);
		msgEnteredVehicle->Write(seat);
		_pServer->m_network.SendMessageToRange(m_position, _pServer->m_settings.StreamingRange, msgEnteredVehicle, m_peer);

		return;
	}

	if (message->m_type == NMT_LeftVehicle) {
		NetHandle vehicleHandle;
		int seat;

		message->Read(vehicleHandle);
		message->Read(seat);

		Vehicle* vehicle = _pServer->m_world.GetEntityFromHandle<Vehicle>(vehicleHandle);
		if (vehicle == nullptr) {
			logWrite("Player tried leaving unknown vehicle!");
			return;
		}

		vehicle->RemoveOccupant(this);

		NetworkMessage* msgEnteredVehicle = new NetworkMessage(NMT_LeftVehicle);
		msgEnteredVehicle->Write(m_handle);
		msgEnteredVehicle->Write(vehicle->m_handle);
		msgEnteredVehicle->Write(seat);
		_pServer->m_network.SendMessageToRange(m_position, _pServer->m_settings.StreamingRange, msgEnteredVehicle, m_peer);

		return;
	}
}

void Player::CheckStreamingEntities()
{
	m_tmLastStreaming = Clock::now();

	std::vector<Entity*> newStreaming;
	_pServer->m_world.QueryRange(m_position, _pServer->m_settings.StreamingRange, newStreaming, this);

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
			msgStreamIn->Write(ent->GetType());
			msgStreamIn->Write(ent->m_handle);
			ent->NetworkSerialize(msgStreamIn);
			ent->AddRef();
		}
		_pServer->m_network.SendMessageTo(m_peer, msgStreamIn);
	}

	if (streamedOut.size() > 0) {
		NetworkMessage* msgStreamOut = new NetworkMessage(NMT_StreamOut);
		msgStreamOut->Write((uint32_t)streamedOut.size());
		for (Entity* ent : streamedOut) {
			msgStreamOut->Write(ent->m_handle);
			ent->Release();
		}
		_pServer->m_network.SendMessageTo(m_peer, msgStreamOut);
	}
}

NetworkEntityType Player::GetType()
{
	return ET_Player;
}

NetStructs::CreatePed Player::GetNetworkCreatePedStruct()
{
	NetStructs::CreatePed ret;
	ret.m_model = m_model;
	ret.m_position = m_position;
	ret.m_rotation = m_rotation;
	return ret;
}

void Player::NetworkSerialize(NetworkMessage* message)
{
	message->Write(GetNetworkCreatePedStruct());
	message->Write(m_username);
	message->Write(m_nickname);
}

void Player::Update()
{
	if (m_peer == nullptr) {
		return;
	}

	if ((int)ClockDuration(Clock::now() - m_tmLastStreaming).count() > 500) {
		CheckStreamingEntities();
	}

	if ((int)ClockDuration(Clock::now() - m_tmSyncLastPosition).count() > 250) {
		m_tmSyncLastPosition = Clock::now();

		//TODO: Only send this when player has moved, and only when not in vehicle

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
