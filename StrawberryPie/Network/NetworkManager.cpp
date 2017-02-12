#include <Common.h>

#include <Network/NetworkManager.h>

#include <Utils.h>

#include <enet/enet.h>

#include <shv/natives.h>

static void networkMessageFree(ENetPacket* packet)
{
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

void NetworkManager::SendToHost(NetworkMessage* message)
{
	m_outgoingMessages.push(message);
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

	//TODO: Make a thread just for network message queueing
	ENetEvent ev;
	while (enet_host_service(m_localHost, &ev, 0) > 0) {
		if (ev.type == ENET_EVENT_TYPE_CONNECT) {
			//m_localPeer = ev.peer;
			m_connected = true;

			UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~g~Connected");
			UI::_DRAW_NOTIFICATION(false, true);

		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			m_localPeer = nullptr;
			m_connected = false;

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

		logWrite("Incoming message of size %u at %p (type %d)", message->m_length, message->m_data, (int)message->m_type);

		if (message->m_type == NMT_Disconnect) {
			std::string reason;
			message->Read(reason);

			UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(reason.c_str());
			UI::_DRAW_NOTIFICATION(false, true);

			enet_peer_disconnect(m_localPeer, 0);
			m_localPeer = nullptr;
		}

		delete message;
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
