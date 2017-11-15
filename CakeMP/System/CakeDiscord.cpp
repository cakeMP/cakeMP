#include <Common.h>

#include <System/CakeDiscord.h>
#include <System/Cake.h>

// Disabled because Discord_Shutdown hangs in std::thread::join().
// IO thread hangs in thread function's epilogue.
// Is it locked by GTA 5 or something? Fucking weird.
#if 0

#include <discord-rpc.h>

NAMESPACE_BEGIN;

CakeDiscord::CakeDiscord()
{
	m_initialized = false;
}

CakeDiscord::~CakeDiscord()
{
	if (m_initialized) {
		Discord_Shutdown();
	}
}

void CakeDiscord::Initialize()
{
	DiscordEventHandlers handlers;
	handlers.ready = []() { _pGame->m_discord.OnReady(); };
	handlers.disconnected = [](int errorCode, const char* message) { _pGame->m_discord.OnDisconnected(errorCode, message); };
	handlers.errored = [](int errorCode, const char* message) { _pGame->m_discord.OnError(errorCode, message); };
	handlers.joinGame = [](const char* joinSecret) { _pGame->m_discord.OnJoinGame(joinSecret); };
	handlers.spectateGame = nullptr;
	handlers.joinRequest = [](const DiscordJoinRequest* request) { _pGame->m_discord.OnJoinRequest(request); };
	Discord_Initialize("380425166829912074", &handlers, 1, nullptr);

	m_initialized = true;
}

void CakeDiscord::Update()
{
	if (m_initialized) {
		Discord_RunCallbacks();
	}
}

void CakeDiscord::GameOnConnected()
{
	std::string strIP = _pGame->m_network.GetServerIP();
	std::string strName = _pGame->m_network.GetServerName();

	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(presence));
	presence.state = "In server";
	presence.details = strName.c_str();
	presence.partyId = strIP.c_str();
	presence.partySize = _pGame->m_network.GetPlayerCount();
	presence.partyMax = _pGame->m_network.GetServerMaxPlayers();
	Discord_UpdatePresence(&presence);
}

void CakeDiscord::GameOnDisconnected()
{
	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(presence));
	presence.state = "In main menu";
	Discord_UpdatePresence(&presence);
}

void CakeDiscord::OnReady()
{
	logWrite("Discord RPC is ready!");

	if (_pGame->m_network.IsConnected()) {
		GameOnConnected();
	} else {
		GameOnDisconnected();
	}
}

void CakeDiscord::OnDisconnected(int errorCode, const char* message)
{
	logWrite("Discord RPC disconnected: %d (%s)", errorCode, message);
}

void CakeDiscord::OnError(int errorCode, const char* message)
{
	logWrite("Discord RPC error: %d (%s)", errorCode, message);
}

void CakeDiscord::OnJoinGame(const char* joinSecret)
{
	logWrite("Discord RPC join game secret: \"%s\"", joinSecret);
}

void CakeDiscord::OnJoinRequest(const DiscordJoinRequest* request)
{
	logWrite("Discord RPC join request received");
}

NAMESPACE_END;

#endif
