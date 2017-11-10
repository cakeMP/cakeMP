#include <Common.h>

#include <Network/NetworkManager.h>

#include <Entities/LocalPlayer.h>
#include <Entities/Vehicle.h>

#include <System/Strawberry.h>
#include <Network/NetHandle.h>
#include <Network/NetworkEntityType.h>
#include <Network/Structs/CreatePed.h>
#include <Network/Structs/CreateVehicle.h>
#include <GTA/UI/UI.h>

#include <enet/enet.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

static void networkMessageFree(ENetPacket* packet)
{
	assert(packet->userData != nullptr);
	assert(((NetworkMessage*)packet->userData)->m_outgoing);
	delete (NetworkMessage*)packet->userData;
}

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
	if (m_localPeer != nullptr) {
		Disconnect();
	}

	if (m_localHost != nullptr) {
		enet_host_destroy(m_localHost);
		enet_deinitialize();
	}
}

void NetworkManager::Connect(const char* hostname, uint16_t port)
{
	if (m_localHost == nullptr || m_localPeer != nullptr) {
		return;
	}

	logWrite("Connecting to %s:%u", hostname, port);

	uiNotify("Connecting..");

	ENetAddress addr;
	enet_address_set_host(&addr, hostname);
	addr.port = port;
	m_localPeer = enet_host_connect(m_localHost, &addr, 1, 0);
}

void NetworkManager::Disconnect()
{
	if (m_localPeer == nullptr) {
		return;
	}

	logWrite("Disconnecting from %08x", m_localPeer->address.host);

	uiNotify("Disconnecting..");

	enet_peer_disconnect(m_localPeer, 0);
	m_localPeer = nullptr;
}

bool NetworkManager::IsConnecting()
{
	return m_localPeer != nullptr && !m_connected;
}

bool NetworkManager::IsConnected()
{
	return m_connected;
}

void NetworkManager::ClearEntities()
{
	for (auto &pair : m_entitiesNetwork) {
		if (pair.second == &_pGame->m_player) {
			continue;
		}
		delete pair.second;
	}
	m_entitiesNetwork.clear();
}

int NetworkManager::GetEntityCount()
{
	return (int)m_entitiesNetwork.size();
}

void NetworkManager::SendToHost(NetworkMessage* message)
{
	if (m_localPeer == nullptr) {
		logWrite("WARNING: Tried sending a network message with type %d while not connected!", (int)message->m_type);
		delete message;
		return;
	}
	m_outgoingMessages.push(message);
}

Entity* NetworkManager::GetEntityFromHandle(NetworkEntityType expectedType, const NetHandle &handle)
{
	auto it = m_entitiesNetwork.find(handle);
	if (it == m_entitiesNetwork.end()) {
		assert(false);
		return nullptr;
	}

	if (it->second->GetType() != expectedType) {
		return nullptr;
	}

	return it->second;
}

Entity* NetworkManager::GetEntityFromLocalHandle(NetworkEntityType expectedType, int handle)
{
	for (auto &pair : m_entitiesNetwork) {
		if (pair.second->GetLocalHandle() == handle && pair.second->GetType() == expectedType) {
			return pair.second;
		}
	}
	return nullptr;
}

void NetworkManager::Initialize()
{
	if (enet_initialize() < 0) {
		logWrite("Failed to initialize ENet!");
		return;
	}

	m_localHost = enet_host_create(nullptr, 1, 1, 0, 0);
}

void NetworkManager::Update()
{
	if (m_localHost == nullptr) {
		return;
	}

	for (auto &pair : m_entitiesNetwork) {
		pair.second->Update();
	}

	uint32_t incomingBytes = 0;

	//TODO: Make a thread just for network message queueing
	ENetEvent ev;
	while (enet_host_service(m_localHost, &ev, 0) > 0) {
		if (ev.type == ENET_EVENT_TYPE_CONNECT) {
			//m_localPeer = ev.peer;
			m_connected = true;

			LocalPlayer &player = _pGame->m_player;

			NetworkMessage* msgHandshake = new NetworkMessage(NMT_Handshake);
			msgHandshake->Write(player.m_username);
			msgHandshake->Write(player.m_nickname);
			SendToHost(msgHandshake);

			//NOTE: We don't call OnConnected until the server has accepted our handshake

		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			m_localPeer = nullptr;
			m_connected = false;

			_pGame->OnDisconnected();

		} else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
			NetworkMessage* newMessage = new NetworkMessage(ev.peer, ev.packet);
			incomingBytes += (uint32_t)newMessage->m_length;
			m_incomingMessages.push(newMessage);
		}
	}

	m_statsIncomingMessagesTotal += m_incomingMessages.size();
	m_statsIncomingMessages.Add((uint32_t)m_incomingMessages.size());
	m_statsIncomingBytes.Add(incomingBytes);
	m_statsIncomingBytesTotal += incomingBytes;

	while (m_incomingMessages.size() > 0) {
		NetworkMessage* message = m_incomingMessages.front();
		m_incomingMessages.pop();

		//logWrite("Incoming message of size %u at %p (type %d)", message->m_length, message->m_data, (int)message->m_type);

		message->m_handled = true;

		HandleMessage(message);

		if (message->m_handled) {
			delete message;
		}
	}

	m_statsOutgoingMessagesTotal += m_outgoingMessages.size();
	m_statsOutgoingMessages.Add((uint32_t)m_outgoingMessages.size());

	uint32_t outgoingBytes = 0;

	while (m_outgoingMessages.size() > 0) {
		NetworkMessage* message = m_outgoingMessages.front();
		m_outgoingMessages.pop();

		outgoingBytes += (uint32_t)message->m_length;

		ENetPacket* newPacket = enet_packet_create(message->m_data, message->m_length, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
		newPacket->userData = message;
		newPacket->freeCallback = networkMessageFree;
		enet_peer_send(m_localPeer, 0, newPacket);

		// Note: We don't delete this packet here, we wait until ENet tells us it's no longer in use and delete it in the free callback (networkMessageFree)
	}

	m_statsOutgoingBytes.Add(outgoingBytes);
	m_statsOutgoingBytesTotal += outgoingBytes;
}

//TODO: Perhaps this should move to some other place?
void NetworkManager::HandleMessage(NetworkMessage* message)
{
	if (message->m_type == NMT_Disconnect) {
		std::string reason;
		message->Read(reason);

		logWrite("Server disconnected us: '%s'", reason.c_str());

		uiNotify(reason);

		enet_peer_disconnect(m_localPeer, 0);
		m_localPeer = nullptr;

		return;
	}

	if (message->m_type == NMT_Handshake) {
		NetHandle handle;
		glm::vec3 position;
		uint32_t skinHash;

		message->Read(handle);
		message->Read(position);
		message->Read(skinHash);

		logWrite("We have received our local handle: %u", handle.m_value);

		LocalPlayer &player = _pGame->m_player;
		player.SetNetHandle(handle);
		player.SetModel(skinHash);
		player.SetPositionNoOffset(position);

		_pGame->OnConnected();

		return;
	}

	if (message->m_type == NMT_StreamIn) {
		logWrite("Streaming in:");

		uint32_t numEntities;
		message->Read(numEntities);

		for (uint32_t i = 0; i < numEntities; i++) {
			NetworkEntityType entityType = ET_None;
			NetHandle entityHandle;

			message->Read(entityType);
			message->Read(entityHandle);

			logWrite("  %u (%d)", entityHandle.m_value, (int)entityType);

			//TODO: Clean this up a bit, it's gonna become huge if we leave this like this
			if (entityType == ET_Player) {
				NetStructs::CreatePed createPedPlayer;
				std::string username, nickname;

				message->Read(createPedPlayer);
				message->Read(username);
				message->Read(nickname);

				Player* newPlayer = new Player(entityHandle, createPedPlayer);
				newPlayer->m_username = username;
				newPlayer->m_nickname = nickname;
				m_entitiesNetwork[entityHandle] = newPlayer;

			} else if (entityType == ET_Vehicle) {
				NetStructs::CreateVehicle createVehicle;

				message->Read(createVehicle);

				Vehicle* newVehicle = new Vehicle(entityHandle, createVehicle);
				m_entitiesNetwork[entityHandle] = newVehicle;

			} else {
				assert(false);
				break;
			}
		}
	}

	if (message->m_type == NMT_StreamOut) {
		logWrite("Streaming out:");

		uint32_t numEntities;
		message->Read(numEntities);

		for (uint32_t i = 0; i < numEntities; i++) {
			NetHandle handle;
			message->Read(handle);

			auto it = m_entitiesNetwork.find(handle);
			if (it == m_entitiesNetwork.end()) {
				assert(false);
				return;
			}

			logWrite("  %u (%d)", handle.m_value, (int)it->second->GetType());

			m_entitiesNetwork.erase(it);
			delete it->second;
		}
	}

	if (message->m_type == NMT_PlayerJoin) {
		NetStructs::CreatePed createPedPlayer;
		std::string username, nickname;

		message->Read(createPedPlayer);
		message->Read(username);
		message->Read(nickname);

		logWrite("Player joined: %s (%s)", username.c_str(), nickname.c_str());

		uiNotify("~b~%s~s~ joined", nickname.c_str());

		return;
	}

	if (message->m_type == NMT_PlayerLeave) {
		NetHandle handle;

		message->Read(handle);

		auto it = m_entitiesNetwork.find(handle);
		if (it == m_entitiesNetwork.end()) {
			assert(false);
			return;
		}

		if (it->second->GetType() != ET_Player) {
			assert(false);
			return;
		}

		Player* player = static_cast<Player*>(it->second);

		logWrite("Player left: %s (%s)", player->m_username.c_str(), player->m_nickname.c_str());

		uiNotify("~b~%s~s~ left", player->m_nickname.c_str());

		return;
	}

	if (message->m_type == NMT_ChatMessage) {
		std::string text;

		message->Read(text);

		_pGame->m_interface.m_chat.AddMessage(text);

		return;
	}

	if (message->m_type == NMT_PlayerMove) {
		NetHandle handle;
		glm::vec3 newPosition, newVelocity;
		float newHeading;
		uint8_t newMoveType;

		message->Read(handle);

		Player* player = GetEntityFromHandle<Player>(ET_Player, handle);
		if (player == nullptr) {
			// Can happen if the player has *just* been streamed out
			return;
		}

		if (player->m_inVehicle) {
			return;
		}

		message->Read(newPosition);
		message->Read(newHeading);
		message->Read(newVelocity);
		message->Read(newMoveType);

		glm::vec3 posOld = player->GetPosition();
		float headingOld = player->GetHeading();
		glm::vec3 posPredict = newPosition + (newPosition - posOld) + newVelocity * 1.25f; // This doesn't look right

		player->m_speedOnFoot = (OnFootMoveTypes)newMoveType;
		player->m_speedOnFootTowards = posPredict;
		if (glm::distance2(posOld, newPosition) > 0.1f) {
			player->InterpolatePosition(posOld, newPosition, 250);
		}
		player->InterpolateHeading(headingOld, newHeading, 250);

		return;
	}

	if (message->m_type == NMT_Weather) {
		std::string weatherType;

		message->Read(weatherType);

		GAMEPLAY::SET_WEATHER_TYPE_PERSIST(weatherType.c_str());

		return;
	}

	if (message->m_type == NMT_ClockTime) {
		int hours, minutes, seconds;

		message->Read(hours);
		message->Read(minutes);
		message->Read(seconds);

		TIME::SET_CLOCK_TIME(hours, minutes, seconds);

		return;
	}

	if (message->m_type == NMT_EnteringVehicle) {
		NetHandle playerHandle, vehicleHandle;
		int seat;

		message->Read(playerHandle);
		message->Read(vehicleHandle);
		message->Read(seat);

		Player* player = GetEntityFromHandle<Player>(ET_Player, playerHandle);
		Vehicle* vehicle = GetEntityFromHandle<Vehicle>(ET_Vehicle, vehicleHandle);

		if (player == nullptr || vehicle == nullptr) {
			logWrite("WARNING: Player %p (%u) tried entering vehicle %p (%u) (entering)", player, (uint32_t)playerHandle, vehicle, (uint32_t)vehicleHandle);
			return;
		}

		player->m_inVehicle = true;
		AI::TASK_ENTER_VEHICLE(player->GetLocalHandle(), vehicle->GetLocalHandle(), 1000, seat, 1.0f, 3, 0);

		return;
	}

	if (message->m_type == NMT_EnteredVehicle) {
		NetHandle playerHandle, vehicleHandle;
		int seat;

		message->Read(playerHandle);
		message->Read(vehicleHandle);
		message->Read(seat);

		Player* player = GetEntityFromHandle<Player>(ET_Player, playerHandle);
		Vehicle* vehicle = GetEntityFromHandle<Vehicle>(ET_Vehicle, vehicleHandle);

		if (player == nullptr || vehicle == nullptr) {
			logWrite("WARNING: Player %p (%u) tried entering vehicle %p (%u) (entered)", player, (uint32_t)playerHandle, vehicle, (uint32_t)vehicleHandle);
			return;
		}

		PED::SET_PED_INTO_VEHICLE(player->GetLocalHandle(), vehicle->GetLocalHandle(), seat);

		return;
	}

	if (message->m_type == NMT_LeftVehicle) {
		NetHandle playerHandle, vehicleHandle;
		int seat;

		message->Read(playerHandle);
		message->Read(vehicleHandle);
		message->Read(seat);

		Player* player = GetEntityFromHandle<Player>(ET_Player, playerHandle);
		Vehicle* vehicle = GetEntityFromHandle<Vehicle>(ET_Vehicle, vehicleHandle);

		if (player == nullptr || vehicle == nullptr) {
			logWrite("WARNING: Player %p (%u) tried leaving vehicle %p (%u)", player, (uint32_t)playerHandle, vehicle, (uint32_t)vehicleHandle);
			return;
		}

		player->m_inVehicle = false;

		//TODO: Somehow wait for this task to finish before allowing movement (like m_inVehicle), to avoid janky animation
		AI::TASK_LEAVE_VEHICLE(player->GetLocalHandle(), vehicle->GetLocalHandle(), 1);

		return;
	}
}

NAMESPACE_END;
