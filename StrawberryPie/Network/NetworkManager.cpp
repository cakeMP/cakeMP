#include <Common.h>

#include <Network/NetworkManager.h>

#include <Utils.h>

#include <enet/enet.h>

#include <shv/natives.h>

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

	ENetEvent ev;
	if (enet_host_service(m_localHost, &ev, 0) > 0) {
		if (ev.type == ENET_EVENT_TYPE_CONNECT) {
			UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~g~Connected");
			UI::_DRAW_NOTIFICATION(false, true);
		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~r~Disconnected");
			UI::_DRAW_NOTIFICATION(false, true);
		}
	}
}
