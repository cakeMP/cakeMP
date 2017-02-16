#include <Common.h>

#include <Network/NetworkManager.h>

#include <Entities/LocalPlayer.h>
#include <System/Strawberry.h>
#include <Network/NetHandle.h>
#include <Network/NetworkEntityType.h>
#include <Network/Structs/CreatePed.h>

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

	UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Connecting..");
	UI::_DRAW_NOTIFICATION(false, true);

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

	UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Disconnecting..");
	UI::_DRAW_NOTIFICATION(false, true);

	enet_peer_disconnect(m_localPeer, 0);
	m_localPeer = nullptr;
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

void NetworkManager::SendToHost(NetworkMessage* message)
{
	if (m_localPeer == nullptr) {
		logWrite("WARNING: Tried sending a network message with type %d while not connected!", (int)message->m_type);
		delete message;
		return;
	}
	m_outgoingMessages.push(message);
}

template<typename T>
T* NetworkManager::GetEntityFromHandle(const NetHandle &handle)
{
	auto it = m_entitiesNetwork.find(handle);
	if (it == m_entitiesNetwork.end()) {
		assert(false);
		return nullptr;
	}

	return dynamic_cast<T*>(it->second);
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

			UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~g~Connected");
			UI::_DRAW_NOTIFICATION(false, true);

		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			m_localPeer = nullptr;
			m_connected = false;

			//TODO: Also delete local entities (m_entitiesLocal in Strawberry?)
			ClearEntities();

			UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~r~Disconnected");
			UI::_DRAW_NOTIFICATION(false, true);

		} else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
			NetworkMessage* newMessage = new NetworkMessage(ev.peer, ev.packet);
			m_incomingMessages.push(newMessage);
		}
	}

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

	while (m_outgoingMessages.size() > 0) {
		NetworkMessage* message = m_outgoingMessages.front();
		m_outgoingMessages.pop();

		ENetPacket* newPacket = enet_packet_create(message->m_data, message->m_length, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
		newPacket->userData = message;
		newPacket->freeCallback = networkMessageFree;
		enet_peer_send(m_localPeer, 0, newPacket);

		// Note: We don't delete this packet here, we wait until ENet tells us it's no longer in use and delete it in the free callback (networkMessageFree)
	}
}

//TODO: Perhaps this should move to some other place?
void NetworkManager::HandleMessage(NetworkMessage* message)
{
	if (message->m_type == NMT_Disconnect) {
		std::string reason;
		message->Read(reason);

		logWrite("Server disconnected us: '%s'", reason.c_str());

		UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(reason.c_str());
		UI::_DRAW_NOTIFICATION(false, true);

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

		_pGame->m_player.SetNetHandle(handle);
		_pGame->m_player.SetModel(skinHash);
		_pGame->m_player.SetPositionNoOffset(position);

		return;
	}

	if (message->m_type == NMT_CreateEntities) {
		logWrite("Creating entities:");

		uint32_t numEntities;
		message->Read(numEntities);

		for (uint32_t i = 0; i < numEntities; i++) {
			NetworkEntityType entityType = ET_None;
			message->Read(entityType);

			logWrite("  %d", (int)entityType);

			//TODO: Clean this up a bit, it's gonna become huge if we leave this like this
			if (entityType == ET_Player) {
				NetStructs::CreatePed createPedPlayer;
				std::string username, nickname;

				message->Read(createPedPlayer);
				message->Read(username);
				message->Read(nickname);

				Player* newPlayer = new Player(createPedPlayer);
				newPlayer->m_username = username;
				newPlayer->m_nickname = nickname;
				m_entitiesNetwork[createPedPlayer.m_handle] = newPlayer;

			} else {
				assert(false);
				break;
			}
		}
	}

	if (message->m_type == NMT_PlayerJoin) {
		NetStructs::CreatePed createPedPlayer;
		std::string username, nickname;

		message->Read(createPedPlayer);
		message->Read(username);
		message->Read(nickname);

		logWrite("Player joined: %s (%s)", username.c_str(), nickname.c_str());

		Player* newPlayer = new Player(createPedPlayer);
		newPlayer->m_username = username;
		newPlayer->m_nickname = nickname;
		m_entitiesNetwork[createPedPlayer.m_handle] = newPlayer;

		std::string joinMessage = "~b~";
		joinMessage += username;
		joinMessage += "~s~ joined";

		UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(joinMessage.c_str());
		UI::_DRAW_NOTIFICATION(false, true);

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

		Player* player = dynamic_cast<Player*>(it->second);
		if (player == nullptr) {
			assert(false);
			return;
		}

		logWrite("Player left: %s (%s)", player->m_username.c_str(), player->m_nickname.c_str());

		std::string leaveMessage = "~b~";
		leaveMessage += player->m_username;
		leaveMessage += "~s~ left";

		UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(leaveMessage.c_str());
		UI::_DRAW_NOTIFICATION(false, true);

		m_entitiesNetwork.erase(it);
		delete player;

		return;
	}

	if (message->m_type == NMT_PlayerMove) {
		NetHandle handle;
		glm::vec3 newPosition, newVelocity;
		float newHeading;
		uint8_t newMoveType;

		message->Read(handle);

		Player* player = GetEntityFromHandle<Player>(handle);
		if (player == nullptr) {
			assert(false);
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
		player->InterpolatePosition(posOld, newPosition, 500);
		player->InterpolateHeading(headingOld, newHeading, 500);

		return;
	}
}

NAMESPACE_END;
