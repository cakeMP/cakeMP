#pragma once

#include <Common.h>

#if 0

struct DiscordJoinRequest;

NAMESPACE_BEGIN;

class CakeDiscord
{
public:
	bool m_initialized;

public:
	CakeDiscord();
	~CakeDiscord();

	void Initialize();
	void Update();

	void GameOnConnected();
	void GameOnDisconnected();

	void OnReady();
	void OnDisconnected(int errorCode, const char* message);
	void OnError(int errorCode, const char* message);
	void OnJoinGame(const char* joinSecret);
	void OnJoinRequest(const DiscordJoinRequest* request);
};

NAMESPACE_END;

#endif
