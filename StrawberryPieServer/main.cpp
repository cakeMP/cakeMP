#include <cstdio>

#include <enet/enet.h>

#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "WinMM.lib")

// This code is **ugly**, and just for testing. Tell Miss to fix this!

int main()
{
	enet_initialize();

	int tickRate = 60;
	size_t maxClients = 32;
	size_t maxChannels = 1;

	ENetAddress serverAddr;
	serverAddr.host = ENET_HOST_ANY;
	serverAddr.port = 4499;

	ENetHost* host = enet_host_create(&serverAddr, maxClients, maxChannels, 0, 0);
	if (host == nullptr) {
		printf("Couldn't create server host!\n");
		return 1;
	}

	printf("Listening on port %d\n", serverAddr.port);

	while (true) {
		ENetEvent ev;
		if (enet_host_service(host, &ev, 0) > 0) {
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				printf("-- New connection from %08x:%d\n", ev.peer->address.host, ev.peer->address.port);
			} else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
				printf("-- Disconnected from %08x:%d\n", ev.peer->address.host, ev.peer->address.port);
			}
		}

		Sleep(1000 / tickRate);
	}

	enet_deinitialize();
	return 0;
}
