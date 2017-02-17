#include <Common.h>

#include <cmath>

#include <signal.h>

#include <enet/enet.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Network/NetHandle.h>
#include <Network/NetworkMessage.h>

static volatile bool g_keepRunning = true;

static void IntHandler(int dummy)
{
	g_keepRunning = false;
}

static ENetHost* g_host = nullptr;
static ENetPeer* g_peer = nullptr;

static void MessageFreeCallback(ENetPacket* packet)
{
	delete (NetworkMessage*)packet->userData;
}

static void SendToHost(NetworkMessage* msg)
{
	ENetPacket* packet = enet_packet_create(msg->m_data, msg->m_length, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
	packet->userData = msg;
	packet->freeCallback = MessageFreeCallback;
	enet_peer_send(g_peer, 0, packet);
}

int main()
{
	signal(SIGINT, IntHandler);

	enet_initialize();

	g_host = enet_host_create(nullptr, 1, 1, 0, 0);

	ENetAddress addr;
	enet_address_set_host(&addr, "localhost");
	addr.port = 4499;
	g_peer = enet_host_connect(g_host, &addr, 1, 0);

	bool connected = false;

	glm::vec3 spawnPoint;
	glm::vec3 prevPos;
	int sendPosC = 0;
	int tickCount = 0;

	while (g_keepRunning) {
		ENetEvent ev;
		if (enet_host_service(g_host, &ev, 0) > 0) {
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				printf("Connected!\n");
				connected = true;

				NetworkMessage* msgHandshake = new NetworkMessage(NMT_Handshake);
				msgHandshake->Write("Emu");
				msgHandshake->Write("Emu");
				SendToHost(msgHandshake);

				continue;
			}

			if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
				printf("Disconnected!\n");
				connected = false;
				break;
			}

			if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
				NetworkMessage* msg = new NetworkMessage(ev.peer, ev.packet);

				if (msg->m_type == NMT_Disconnect) {
					std::string reason;
					msg->Read(reason);

					printf("Server kicked us: %s\n", reason.c_str());
					enet_peer_disconnect(g_peer, 0);

					delete msg;
					break;

				} else if (msg->m_type == NMT_Handshake) {
					NetHandle handle;
					glm::vec3 position;
					uint32_t skin;

					msg->Read(handle);
					msg->Read(position);
					msg->Read(skin);

					spawnPoint = position;
					prevPos = spawnPoint;

					printf("Handshake received from server:\n");
					printf("  Our NetHandle is: %08X\n", handle.m_value);
					printf("  Our position is: %f, %f, %f\n", position.x, position.y, position.z);
					printf("  Our skin is: %08X\n", skin);

				} else if (msg->m_type == NMT_CreateEntities) {
					uint32_t numEntities;

					msg->Read(numEntities);

					printf("NMT_CreateEntities received:\n");
					printf("  Number of entities: %u\n", numEntities);

				} else {
					printf("Other packet received: %d\n", msg->m_type);
				}

				delete msg;
				continue;
			}
		}

		if (++sendPosC >= 7) {
			printf("Send position!\n");
			tickCount++;

			float distance = 4.0f;
			float dx = cosf(tickCount / 5.0f) * distance;
			float dy = sinf(tickCount / 5.0f) * distance;
			glm::vec3 newPos = spawnPoint + glm::vec3(dx, dy, 0);
			glm::vec3 diff = glm::normalize(newPos - prevPos);
			float heading = glm::degrees(atan2f(diff.y, diff.x)) - 90.0f;

			prevPos = newPos;

			NetworkMessage* msgPos = new NetworkMessage(NMT_PlayerMove);
			msgPos->Write(spawnPoint + glm::vec3(dx, dy, 0));
			msgPos->Write(heading);
			msgPos->Write(diff * 4.0f);
			msgPos->Write((uint8_t)3);
			SendToHost(msgPos);

			sendPosC = 0;
		}

		usleep(17 * 1000);
	}

	if (connected) {
		printf("Disconnecting now..\n");
		enet_peer_disconnect_now(g_peer, 0);
	}

	enet_host_destroy(g_host);

	enet_deinitialize();
	return 0;
}
