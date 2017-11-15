#pragma once

#include <Common.h>

#include <Network/NetworkManager.h>

#include <System/Settings.h>
#include <System/Interface/Interface.h>
//#include <System/CakeDiscord.h>

#include <Entities/LocalPlayer.h>

NAMESPACE_BEGIN;

class Cake
{
public:
	HMODULE m_hInstance;
	//HWND m_hWnd = nullptr;
	uint8_t m_keyStates[256];

	//CakeDiscord m_discord;

	NetworkManager m_network;
	Settings m_settings;

	LocalPlayer m_player;

	Interface m_interface;

	int m_gameTime = 0;

	uint32_t m_pedRelGroup = 0;

public:
	Cake(HMODULE hInstance);
	~Cake();

	void Initialize();
	void Update(float dt);
	void Frame();

	void OnConnected();
	void OnDisconnected();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);

	bool IsKeyDown(uint8_t key);
};

extern Cake* _pGame;

NAMESPACE_END;
