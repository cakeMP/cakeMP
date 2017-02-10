#include <Common.h>

#include <Network/NetworkManager.h>

NetworkManager::NetworkManager()
{
	if (enet_initialize() < 0) {
		printf("Failed to initialize ENet!\n");
	}
}

NetworkManager::~NetworkManager()
{
	Close();

	enet_deinitialize();
}

void NetworkManager::Listen(const char* host, uint16_t port, uint32_t maxClients)
{
	if (m_hostListen != nullptr) {
		Close();
	}

	ENetAddress addr;
	enet_address_set_host(&addr, host);
	addr.port = port;
	m_hostListen = enet_host_create(&addr, maxClients, 1, 0, 0);

	printf("Server is listening on %s:%d\n", host, port);
	printf("Max clients: %d\n", maxClients);
}

void NetworkManager::Close()
{
	if (m_hostListen == nullptr) {
		return;
	}

	printf("Closing listen host\n");

	enet_host_destroy(m_hostListen);
	m_hostListen = nullptr;
}

void NetworkManager::Update()
{
	if (m_hostListen == nullptr) {
		return;
	}

	ENetEvent ev;
	if (enet_host_service(m_hostListen, &ev, 0) > 0) {
		if (ev.type == ENET_EVENT_TYPE_CONNECT) {
			printf("New connection from: %08x:%d\n", ev.peer->address.host, ev.peer->address.port);
		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			printf("Client disconnected: %08x\n", ev.peer->address.host);
		}
	}
}
