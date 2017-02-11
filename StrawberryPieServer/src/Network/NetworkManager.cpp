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

Player* NetworkManager::FindPlayer(const ENetPeer* peer)
{
	//TODO: This function is possibly slow, but I don't see any userdata field on peers, so this will have to do for now..
	for (Player* p : m_players) {
		//TODO: Does ENet throw the objects away/around that we can't compare actual pointers?
		//NOTE: connectID can not be used here because when a disconnect event is received, only the peer's data is valid!
		if (p->GetPeer() == peer) {
			return p;
		}
	}
	return nullptr;
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

	while (m_players.size() > 0) {
		delete m_players[m_players.size() - 1];
		m_players.pop_back();
	}

	printf("Closing listen host\n");

	enet_host_destroy(m_hostListen);
	m_hostListen = nullptr;
}

void NetworkManager::SendMessageTo(ENetPeer* peer, NetworkMessage* message)
{
	message->m_forPeer = peer;
	m_outgoingMessages.push(message);
}

void NetworkManager::Update()
{
	if (m_hostListen == nullptr) {
		return;
	}

	ENetEvent ev;
	if (enet_host_service(m_hostListen, &ev, 0) > 0) {
		if (ev.type == ENET_EVENT_TYPE_CONNECT) {
			printf("New connection from: %08x:%d (id %d)\n", ev.peer->address.host, ev.peer->address.port, ev.peer->connectID);

			Player* newPlayer = new Player(ev.peer);
			m_players.push_back(newPlayer);

			newPlayer->OnConnected();

		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			printf("Client disconnected: %08x\n", ev.peer->address.host);

			Player* player = FindPlayer(ev.peer);
			assert(player != nullptr);
			if (player != nullptr) {
				m_players.erase(std::find(m_players.begin(), m_players.end(), player));
				player->OnDisconnected();
				delete player;
			}

		} else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
			NetworkMessage* newMessage = new NetworkMessage(ev.peer, ev.packet);
			m_incomingMessages.push(newMessage);
		}
	}

	size_t frameReceived = 0;
	size_t frameSent = 0;

	while (m_incomingMessages.size() > 0) {
		NetworkMessage* message = m_incomingMessages.front();
		m_incomingMessages.pop();

		frameReceived += message->m_length;

		//TODO: Process incoming messages
		printf("Incoming message at %p of size %u for peer %d\n", message->m_data, message->m_length, message->m_forPeer->connectID);

		delete message;
	}

	while (m_outgoingMessages.size() > 0) {
		NetworkMessage* message = m_outgoingMessages.front();
		m_outgoingMessages.pop();

		frameSent += message->m_length;

		uint32_t packetFlags = ENET_PACKET_FLAG_NO_ALLOCATE;

		if (message->Reliable()) {
			packetFlags |= ENET_PACKET_FLAG_RELIABLE;
		}

		ENetPacket* newPacket = enet_packet_create(message->m_data, message->m_length, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
		newPacket->userData = message;
		newPacket->freeCallback = networkMessageFree;
		enet_peer_send(message->m_forPeer, 0, newPacket);

		// Note: We don't delete this packet here, we wait until ENet tells us it's no longer in use and delete it in the free callback (networkMessageFree)
	}
}
