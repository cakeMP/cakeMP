#include <Common.h>

#include <Network/NetworkManager.h>

#include <GameServer.h>

static void networkMessageFree(ENetPacket* packet)
{
	assert(packet->userData != nullptr);
	assert(((NetworkMessage*)packet->userData)->m_outgoing);
	delete (NetworkMessage*)packet->userData;
}

NetworkManager::NetworkManager(GameServer* server)
{
	m_server = server;

	if (enet_initialize() < 0) {
		logWrite("Failed to initialize ENet!");
	}
}

NetworkManager::~NetworkManager()
{
	Close();

	enet_deinitialize();
}

NetHandle NetworkManager::AssignHandle()
{
	//TODO: This could be smarter in the future so it uses unused slots
	return NetHandle(m_handleIterator++);
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

	logWrite("Server is listening on %s:%d", host, port);
	logWrite("Max clients: %d", maxClients);
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

	logWrite("Closing listen host");

	enet_host_destroy(m_hostListen);
	m_hostListen = nullptr;
}

void NetworkManager::OnClientConnect(ENetPeer* peer)
{
	logWrite("New connection from: %08x:%d", peer->address.host, peer->address.port);

	NetHandle newPlayerHandle = AssignHandle();

	Player* newPlayer = new Player(peer, newPlayerHandle);
	peer->data = newPlayer;
	m_players.push_back(newPlayer);

	m_server->m_world.AddEntity(newPlayer);

	newPlayer->OnConnected();
}

void NetworkManager::OnClientDisconnect(ENetPeer* peer)
{
	logWrite("Client disconnected: %08x", peer->address.host);

	Player* player = (Player*)peer->data;
	assert(player != nullptr);
	if (player != nullptr) {
		m_players.erase(std::find(m_players.begin(), m_players.end(), player));
		m_server->m_world.RemoveEntity(player);
		player->OnDisconnected();
		player->Release();
	}
}

void NetworkManager::SendMessageTo(ENetPeer* peer, NetworkMessage* message)
{
	message->m_handled = false;
	message->m_outgoing = true;
	message->m_forPeer = peer;
	message->m_exceptPeer = nullptr;
	m_outgoingMessages.push(message);
}

void NetworkManager::SendMessageToAll(NetworkMessage* message)
{
	message->m_handled = false;
	message->m_outgoing = true;
	message->m_forPeer = nullptr;
	message->m_exceptPeer = nullptr;
	m_outgoingMessages.push(message);
}

void NetworkManager::SendMessageToAll(NetworkMessage* message, ENetPeer* except)
{
	message->m_handled = false;
	message->m_outgoing = true;
	message->m_forPeer = nullptr;
	message->m_exceptPeer = except;
	m_outgoingMessages.push(message);
}

void NetworkManager::SendMessageToRange(const glm::vec3 &pos, float range, NetworkMessage* message)
{
	message->m_handled = false;
	message->m_outgoing = true;
	message->m_forPeer = nullptr;
	message->m_exceptPeer = nullptr;
	message->m_emitPosition = pos;
	message->m_emitRange = (uint32_t)(range * range);
	m_outgoingMessages.push(message);
}

void NetworkManager::SendMessageToRange(const glm::vec3 &pos, float range, NetworkMessage* message, ENetPeer* except)
{
	message->m_handled = false;
	message->m_outgoing = true;
	message->m_forPeer = nullptr;
	message->m_exceptPeer = except;
	message->m_emitPosition = pos;
	message->m_emitRange = range;
	m_outgoingMessages.push(message);
}

void NetworkManager::Update()
{
	if (m_hostListen == nullptr) {
		return;
	}

	//TODO: Make a thread just for network message queueing
	ENetEvent ev;
	while (enet_host_service(m_hostListen, &ev, 0) > 0) {
		if (ev.type == ENET_EVENT_TYPE_CONNECT) {
			OnClientConnect(ev.peer);
		} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
			OnClientDisconnect(ev.peer);
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

		message->m_handled = true;

		Player* forPlayer = (Player*)message->m_forPeer->data;
		forPlayer->HandleMessage(message);

		if (message->m_handled) {
			delete message;
		}
	}

	while (m_outgoingMessages.size() > 0) {
		NetworkMessage* message = m_outgoingMessages.front();
		m_outgoingMessages.pop();

		frameSent += message->m_length;

		uint32_t packetFlags = ENET_PACKET_FLAG_NO_ALLOCATE;

		if (message->Reliable()) {
			packetFlags |= ENET_PACKET_FLAG_RELIABLE;
		}

		ENetPacket* newPacket = enet_packet_create(message->m_data, message->m_length, packetFlags);
		newPacket->userData = message;
		newPacket->freeCallback = networkMessageFree;

		if (message->m_forPeer == nullptr) {
			if (message->m_emitRange > 0.0f) {
				//TODO: Consider using the flat m_players array instead. This is faster if there are
				//      a lot of non-player entities in a world node.
				std::vector<Entity*> result;
				m_server->m_world.QueryRange(message->m_emitPosition, message->m_emitRange, result);
				for (Entity* ent : result) {
					Player* player = dynamic_cast<Player*>(ent);
					if (player == nullptr) {
						continue;
					}
					if (message->m_exceptPeer != nullptr && message->m_exceptPeer == player->GetPeer()) {
						continue;
					}
					enet_peer_send(player->GetPeer(), 0, newPacket);
				}
			} else {
				for (Player* player : m_players) {
					if (message->m_exceptPeer != nullptr && message->m_exceptPeer == player->GetPeer()) {
						continue;
					}
					enet_peer_send(player->GetPeer(), 0, newPacket);
				}
			}
		} else {
			enet_peer_send(message->m_forPeer, 0, newPacket);
		}

		// Note: We don't delete this packet here, we wait until ENet tells us it's no longer in use and delete it in the free callback (networkMessageFree)
	}
}
