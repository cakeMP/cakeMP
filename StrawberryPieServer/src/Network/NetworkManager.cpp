#include <Common.h>

#include <Network/NetworkManager.h>

static void networkMessageFree(ENetPacket* packet)
{
	assert(packet->userData != nullptr);
	assert(((NetworkMessage*)packet->userData)->m_outgoing);
	delete (NetworkMessage*)packet->userData;
}

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

void NetworkManager::SendMessageTo(uint32_t peer, NetworkMessage* message)
{
	message->m_forPeer = peer;
	m_outgoingMessages.push_back(message);
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
		} else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
			NetworkMessage* newMessage = new NetworkMessage(ev.peer->connectID, ev.packet);
			m_incomingMessages.push_back(newMessage);
		}
	}

	while (m_incomingMessages.size() > 0) {
		NetworkMessage* message = m_incomingMessages[0];

		//TODO: Process incoming messages

		delete message;
	}

	while (m_outgoingMessages.size() > 0) {
		NetworkMessage* message = m_outgoingMessages[0];

		ENetPacket* newPacket = enet_packet_create(message->m_data, message->m_length, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
		newPacket->userData = message;
		newPacket->freeCallback = networkMessageFree;
		//TODO: Get the peer and actually send the packet
		//enet_peer_send();

		// Note: We don't delete this packet here, we wait until ENet tells us it's no longer in use and delete it in the free callback (networkMessageFree)
	}
}
