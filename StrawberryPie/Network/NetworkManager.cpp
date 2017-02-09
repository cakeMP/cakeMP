#include <Common.h>

#include <Network/NetworkManager.h>

#include <Utils.h>

#include <enet/enet.h>

NetworkManager::NetworkManager()
{
	if (enet_initialize() < 0) {
		logWrite("Failed to initialize ENet!");
		return;
	}

	m_localHost = enet_host_create(nullptr, 1, 1, 0, 0);
}

NetworkManager::~NetworkManager()
{
	if (m_localPeer != nullptr) {
		Disconnect();
	}

	enet_host_destroy(m_localHost);
	enet_deinitialize();
}

void NetworkManager::Connect(const char* hostname, uint16_t port)
{
	if (m_localPeer != nullptr) {
		Disconnect();
	}

	logWrite("Connecting to %08x", m_localPeer->address.host);

	ENetAddress addr;
	enet_address_set_host(&addr, hostname);
	addr.port = port;
	enet_host_connect(m_localHost, &addr, 1, 0);
}

void NetworkManager::Disconnect()
{
	if (m_localPeer == nullptr) {
		return;
	}

	logWrite("Disconnecting from %08x", m_localPeer->address.host);

	enet_peer_disconnect(m_localPeer, 0);
	m_localPeer = nullptr;
}

void NetworkManager::Update()
{
	ENetEvent ev;
	if (enet_host_service(m_localHost, &ev, 0) > 0) {
		logWrite("Network event type %d", (int)ev.type);
	}
}
